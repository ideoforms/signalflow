#include "signal/node/fft/fft.h"

#include <assert.h>

namespace libsignal
{

FFT::FFT(NodeRef input, int fft_size, int hop_size, int window_size, bool do_window)
    : FFTNode(fft_size, hop_size, window_size, do_window), input(input)
{
    this->name = "fft";

    this->add_input("input", this->input);

#ifdef __APPLE__
    /*------------------------------------------------------------------------
     * Initial FFT setup.
     *-----------------------------------------------------------------------*/
    this->log2N = (int) log2((float) fft_size);
    this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);
    /*------------------------------------------------------------------------
     * Temp buffers for FFT calculations.
     *-----------------------------------------------------------------------*/
    this->buffer2 = new sample[fft_size]();
#else
    this->fftw_buffer = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * (this->num_bins + 1));
#endif

    this->buffer = new sample[fft_size]();

    /*------------------------------------------------------------------------
     * To perform an FFT, we have to enqueue at least `fft_size` samples.
     * input_buffer stores our backlog, so make sure we've allocated enough space.
     * input_buffer_size records the current number of frames we have buffered.
     *-----------------------------------------------------------------------*/
    this->input_buffer = new sample[SIGNAL_MAX_FFT_SIZE * 2]();
    this->input_buffer_size = 0;

    /*------------------------------------------------------------------------
     * Hann window for overlap/add
     *-----------------------------------------------------------------------*/
    this->window = new sample[fft_size]();

    if (do_window)
    {
#ifdef __APPLE__
        vDSP_hann_window(this->window, this->window_size, vDSP_HANN_DENORM);
#else
        for (int i = 0; i < this->window_size; i++)
        {
            this->window[i] = 0.5 - 0.5 * cosf(i * M_PI * 2.0 / this->window_size);
        }
#endif
    }
    else
    {
        for (int i = 0; i < this->window_size; i++)
        {
            this->window[i] = 1.0;
        }
    }
}

FFT::~FFT()
{
#ifdef __APPLE__
    vDSP_destroy_fftsetup(this->fft_setup);
    delete this->buffer2;
#endif
    delete this->buffer;
    delete this->input_buffer;
    delete this->window;
}

void FFT::fft(sample *in, sample *out, bool polar, bool do_window)
{
#ifdef __APPLE__
    DSPSplitComplex buffer_split = { buffer, buffer + fft_size / 2 };
    DSPSplitComplex output_split = { out, out + fft_size / 2 };

    /*------------------------------------------------------------------------
     * Apply window
     *-----------------------------------------------------------------------*/
    vDSP_vmul(in, 1, this->window, 1, buffer2, 1, fft_size);

    /*------------------------------------------------------------------------
     * Convert from interleaved format (sequential pairs) to split format,
     * as required by the vDSP real-to-complex functions.
     *
     * Stride of 2 is the smallest permissible value, denoting no gap
     * between "pairs" of samples.
     *-----------------------------------------------------------------------*/
    vDSP_ctoz((DSPComplex *) buffer2, 2, &buffer_split, 1, fft_size / 2);

    /*------------------------------------------------------------------------
     * Perform single-precision FFT, in-place (overwriting real values).
     *-----------------------------------------------------------------------*/
    vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_FORWARD);

    /*------------------------------------------------------------------------
     * Now, calculate magnitudes and phases, stored in our output buffer.
     *-----------------------------------------------------------------------*/

    /*------------------------------------------------------------------------
     * 1. Sending polar values
     *-----------------------------------------------------------------------*/
    if (polar)
    {
        vDSP_ztoc(&buffer_split, 1, (DSPComplex *) buffer2, 2, fft_size / 2);
        vDSP_polar(buffer2, 2, buffer, 2, fft_size / 2);
        vDSP_ctoz((DSPComplex *) buffer, 2, &output_split, 1, fft_size / 2);
    }

    /*------------------------------------------------------------------------
     * 2. Sending cartesian values
     *-----------------------------------------------------------------------*/
    else
    {
        vDSP_ztoc(&buffer_split, 1, (DSPComplex *) out, 2, fft_size / 2);
    }

#else

    /*------------------------------------------------------------------------
     * Apply window
     *-----------------------------------------------------------------------*/
    for (int i = 0; i < this->fft_size; i++)
    {
        this->buffer[i] = in[i] * this->window[i];
    }

    /*------------------------------------------------------------------------
     * Execute FFT
     * TODO: Plan should be initialised in constructor
     *-----------------------------------------------------------------------*/
    fftwf_plan fftw_plan = fftwf_plan_dft_r2c_1d(fft_size, this->buffer, this->fftw_buffer, FFTW_ESTIMATE);
    fftwf_execute(fftw_plan);

    if (polar)
    {
        for (int i = 0; i < this->num_bins; i++)
        {
            float re = fftw_buffer[i][0];
            float im = fftw_buffer[i][1];
            float mag = sqrtf(re * re + im * im);
            float phase = atan2(im, re);
            out[i] = mag * 2.0;
            out[this->num_bins + i] = phase;
        }
    }
    else
    {
        throw std::runtime_error("Cartesian output not yet implemented");
    }

#endif
}

void FFT::process(sample **out, int num_frames)
{
    /*------------------------------------------------------------------------
     * Append the incoming buffer onto our input_buffer.
     * Perform repeated window and FFT by stepping forward hop_size frames.
     *-----------------------------------------------------------------------*/
    if (num_frames + this->input_buffer_size > this->get_output_buffer_length())
    {
        fprintf(stderr, "FFT: Moving overlapped segments from previous IFFT output would exceed memory bounds\n");
        throw std::runtime_error("FFT: Moving overlapped segments from previous IFFT output would exceed memory bounds");
    }
    memcpy(this->input_buffer + this->input_buffer_size, this->input->out[0], num_frames * sizeof(sample));
    this->input_buffer_size += num_frames;

    /*------------------------------------------------------------------------
     * Calculate the number of hops to perform.
     * Each hop is stored in an out put channel so we can't have > 32.
     *-----------------------------------------------------------------------*/

    this->num_hops = ceilf((this->input_buffer_size - this->fft_size + 1.0) / this->hop_size);
    if (this->num_hops < 0)
        this->num_hops = 0;
    if (this->num_hops > SIGNAL_MAX_CHANNELS)
    {
        throw std::runtime_error("FFT: Too many hops. Try passing in a smaller audio buffer.");
    }

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        this->fft(this->input_buffer + (hop * this->hop_size),
                  out[hop],
                  true,
                  this->do_window);
    }

    int frames_processed = this->hop_size * this->num_hops;
    int frames_remaining = this->input_buffer_size - frames_processed;

    memcpy(this->input_buffer, this->input_buffer + frames_processed, frames_remaining * sizeof(sample));
    this->input_buffer_size -= frames_processed;
}

}
