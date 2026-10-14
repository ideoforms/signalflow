#include "signalflow/node/fft/ifft.h"
#include "signalflow/core/graph.h"

namespace signalflow
{

IFFT::IFFT(NodeRef input, bool do_window)
    : FFTOpNode(input), do_window(do_window)
{
    this->name = "ifft";

#if defined(FFT_ACCELERATE)
    this->log2N = (int) log2((float) this->fft_size);
    this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);
    /*------------------------------------------------------------------------
     * Buffers used in intermediate FFT calculations.
     *-----------------------------------------------------------------------*/
    this->buffer2 = new sample[this->num_bins * 2]();
#elif defined(FFT_FFTW)
    this->fftw_buffer = (fftwf_complex *) fftwf_malloc(sizeof(fftwf_complex) * this->num_bins);
#endif

    this->buffer = new sample[this->num_bins * 2]();

    /*------------------------------------------------------------------------
     * Generate a Hann window for overlap-add.
     * Needs to be fft_size, not window_size, because any samples outside
     * of the window_size should be zero'd.
     *-----------------------------------------------------------------------*/
    this->window = new sample[this->fft_size]();

    if (this->do_window)
    {
#if defined(FFT_ACCELERATE)
        vDSP_hann_window(this->window, this->fft_size, vDSP_HANN_NORM);
#elif defined(FFT_FFTW)
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

    /*------------------------------------------------------------------------
     * The output buffer holds the current block plus pending overlap-add
     * output, which extends up to fft_size samples beyond the block.
     *-----------------------------------------------------------------------*/
    int block_size = SIGNALFLOW_NODE_BUFFER_SIZE;
    if (this->graph && this->graph->get_config().get_output_buffer_size())
    {
        block_size = this->graph->get_config().get_output_buffer_size();
    }
    this->output_buffer_length = std::max(this->output_buffer_length, block_size) + this->fft_size;
    this->resize_output_buffers(this->num_output_channels_allocated);
}

IFFT::~IFFT()
{
#if defined(FFT_ACCELERATE)
    vDSP_destroy_fftsetup(this->fft_setup);
    delete this->buffer2;
#endif
    delete this->buffer;
    delete this->window;
}

void IFFT::ifft(sample *in, sample *out, bool polar, bool do_window, float scale_factor)
{
#if defined(FFT_ACCELERATE)
    /*------------------------------------------------------------------------
     * Set up pointers to memory spaces so we can treat input and buffer
     * as split-valued complex pairs.
     *-----------------------------------------------------------------------*/
    DSPSplitComplex buffer_split = { buffer, buffer + num_bins };
    DSPSplitComplex input_split = { in, in + num_bins };

    /*------------------------------------------------------------------------
     * Convert magnitude/phase to complex values.
     * Received values are split but vDSP_rect requires that pairs be
     * given sequentially, thus do a small dance.
     *-----------------------------------------------------------------------*/
    vDSP_ztoc(&input_split, 1, (DSPComplex *) this->buffer, 2, num_bins);
    vDSP_rect(this->buffer, 2, this->buffer2, 2, num_bins);

    /*------------------------------------------------------------------------
     * 1. Expecting polar values
     *-----------------------------------------------------------------------*/
    if (polar)
    {
        vDSP_ctoz((DSPComplex *) this->buffer2, 2, &buffer_split, 1, num_bins);
    }

    /*------------------------------------------------------------------------
     * 2. Expecting Cartesian values
     *-----------------------------------------------------------------------*/
    else
    {
        vDSP_ctoz((DSPComplex *) this->input->out[0], 2, &buffer_split, 1, num_bins);
    }

    /*------------------------------------------------------------------------
     * Perform inverse FFT
     *-----------------------------------------------------------------------*/
    float nyquist = buffer_split.realp[num_bins - 1];
    buffer_split.imagp[0] = nyquist;
    vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_INVERSE);
    vDSP_ztoc(&buffer_split, 1, (DSPComplex *) this->buffer2, 2, num_bins);

    /*------------------------------------------------------------------------
     * Scale down (Required by vDSP)
     *-----------------------------------------------------------------------*/
    float scale = scale_factor / (fft_size * 2.0);
    vDSP_vsmul(buffer2, 1, &scale, buffer2, 1, fft_size);

    /*------------------------------------------------------------------------
     * Apply Hann window (for overlap-add)
     *-----------------------------------------------------------------------*/
    if (do_window)
    {
        vDSP_vmul(buffer2, 1, this->window, 1, buffer2, 1, fft_size);
    }

    /*------------------------------------------------------------------------
     * Add to output buffer (for overlap/add)
     *-----------------------------------------------------------------------*/
    vDSP_vadd(buffer2, 1, out, 1, out, 1, fft_size);

#elif defined(FFT_FFTW)

    // fftw3f
    float *fftw_buffer_floats = (float *) this->fftw_buffer;
    float *mags = in;
    float *phases = in + this->num_bins;
    for (int i = 0; i < num_bins; i++)
    {
        fftw_buffer_floats[i * 2] = mags[i] * cosf(phases[i]);
        fftw_buffer_floats[i * 2 + 1] = mags[i] * sinf(phases[i]);
    }

    fftwf_plan pi = fftwf_plan_dft_c2r_1d(fft_size, (fftwf_complex *) fftw_buffer_floats, this->buffer, FFTW_ESTIMATE);
    fftwf_execute(pi);
    fftwf_destroy_plan(pi);

    /*------------------------------------------------------------------------
     * Apply window and scale down (fftw IFFT output is scaled up by
     * fft_size). Add the scaled output to the out buffer for overlap-add.
     *-----------------------------------------------------------------------*/
    for (int i = 0; i < fft_size; i++)
    {
        this->buffer[i] *= this->window[i];
        this->buffer[i] = this->buffer[i] / (2 * fft_size);
        out[i] += this->buffer[i];
    }
#endif
}

static int gcd(int a, int b)
{
    while (b)
    {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

void IFFT::process(Buffer &out, int num_frames)
{
    /*------------------------------------------------------------------------
     * Move data written in previous calls to process() to the front of the
     * output buffer. Zero anything after that point.
     *-----------------------------------------------------------------------*/
    int buffer_length = this->get_output_buffer_length();
    memmove(this->out[0], this->out[0] + num_frames, (buffer_length - num_frames) * sizeof(sample));
    memset(this->out[0] + buffer_length - num_frames, 0, num_frames * sizeof(sample));
    if (this->has_received_frame)
    {
        this->next_frame_offset -= num_frames;
    }

    /*------------------------------------------------------------------------
     *
     *-----------------------------------------------------------------------*/
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    if (this->num_hops > 0 && !this->has_received_frame)
    {
        /*------------------------------------------------------------------------
         * Frame k covers input samples [k * hop_size, k * hop_size + fft_size),
         * and is output from k * hop_size + latency. Frames must be placed at
         * this fixed position rather than relative to the block in which they
         * arrive, as when the block size is not a multiple of hop_size, the
         * number of frames per block varies.
         *
         * latency = fft_size - gcd(num_frames, hop_size, fft_size) is the
         * smallest latency at which every frame arrives before its output is
         * due (for a constant block size). This gives the same latency as
         * before for block sizes that divide into hop_size.
         *-----------------------------------------------------------------------*/
        int latency = this->fft_size - gcd(gcd(num_frames, this->hop_size), this->fft_size);
        this->next_frame_offset = latency - this->frames_elapsed;
        this->has_received_frame = true;
    }

    if (this->next_frame_offset < 0 || this->next_frame_offset + (this->num_hops - 1) * this->hop_size + this->fft_size > buffer_length)
    {
        throw std::runtime_error("IFFT: Frame position exceeds output buffer (block size must not change while running)");
    }

    /*------------------------------------------------------------------------
     * Perform repeated inverse FFT, moving forward hop_size frames per
     * hop.
     *-----------------------------------------------------------------------*/
    for (int hop = 0; hop < this->num_hops; hop++)
    {
        float scale_factor = (float) hop_size / fft_size;
        // TODO: This really needs resolving
        // scale_factor = (float) num_frames / fft_size;
        this->ifft(this->input->out[hop],
                   this->out[0] + this->next_frame_offset,
                   true,
                   this->do_window,
                   scale_factor);
        this->next_frame_offset += this->hop_size;
    }

    if (!this->has_received_frame)
    {
        this->frames_elapsed += num_frames;
    }

    if (&out != &this->out)
    {
        memcpy(out[0], this->out[0], num_frames * sizeof(sample));
    }
}

void IFFT::trigger(std::string name, float value)
{
    /*------------------------------------------------------------------------
     * Zero the contents of the current output buffer to prevent overlap.
     *-----------------------------------------------------------------------*/
    if (name == "flush")
    {
        int buffer_size_bytes = this->get_output_buffer_length() * sizeof(sample);
        memset(this->out[0], 0, buffer_size_bytes);
    }
}

}
