#ifdef __APPLE__

#include "signal/node/fft/ifft.h"

#include <Accelerate/Accelerate.h>

namespace libsignal
{

IFFT::IFFT(NodeRef input, bool do_window)
    : FFTOpNode(input), do_window(do_window)
{
    this->name = "ifft";

    this->log2N = (int) log2((float) this->fft_size);
    this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

    /*------------------------------------------------------------------------
     * Buffers used in intermediate FFT calculations.
     *-----------------------------------------------------------------------*/
    this->buffer = new sample[this->fft_size]();
    this->buffer2 = new sample[this->fft_size]();

    /*------------------------------------------------------------------------
     * Generate a Hann window for overlap-add.
     *-----------------------------------------------------------------------*/
    this->window = new sample[this->fft_size]();
    vDSP_hann_window(this->window, this->fft_size, vDSP_HANN_NORM);
}

IFFT::~IFFT()
{
    vDSP_destroy_fftsetup(this->fft_setup);
    delete this->buffer;
    delete this->buffer2;
    delete this->window;
}

void IFFT::ifft(sample *in, sample *out, bool polar, bool do_window, float scale_factor)
{
    /*------------------------------------------------------------------------
     * Set up pointers to memory spaces so we can treat input and buffer
     * as split-valued complex pairs.
     *-----------------------------------------------------------------------*/
    DSPSplitComplex buffer_split = { buffer, buffer + fft_size / 2 };
    DSPSplitComplex input_split = { in, in + fft_size / 2 };

    /*------------------------------------------------------------------------
     * Convert magnitude/phase to complex values.
     * Received values are split but vDSP_rect requires that pairs be
     * given sequentially, thus do a small dance.
     *-----------------------------------------------------------------------*/
    vDSP_ztoc(&input_split, 1, (DSPComplex *) this->buffer, 2, fft_size / 2);
    vDSP_rect(this->buffer, 2, this->buffer2, 2, fft_size / 2);

    /*------------------------------------------------------------------------
     * 1. Expecting polar values
     *-----------------------------------------------------------------------*/
    if (polar)
    {
        vDSP_ctoz((DSPComplex *) this->buffer2, 2, &buffer_split, 1, fft_size / 2);
    }

    /*------------------------------------------------------------------------
     * 2. Expecting Cartesian values
     *-----------------------------------------------------------------------*/
    else
    {
        vDSP_ctoz((DSPComplex *) this->input->out[0], 2, &buffer_split, 1, fft_size / 2);
    }

    /*------------------------------------------------------------------------
     * Perform inverse FFT
     *-----------------------------------------------------------------------*/
    vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_INVERSE);
    vDSP_ztoc(&buffer_split, 1, (DSPComplex *) this->buffer2, 2, fft_size / 2);

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
}

void IFFT::process(sample **out, int num_frames)
{
    /*------------------------------------------------------------------------
     * Move data written in previous calls to process() to the front of the
     * output buffer. Zero anything after that point.
     *-----------------------------------------------------------------------*/
    int previous_offset = num_frames;
    int previous_overflow = this->fft_size;
    int previous_overflow_bytes = previous_overflow * sizeof(sample);
    memmove(this->out[0], this->out[0] + previous_offset, previous_overflow_bytes);
    int buffer_size_bytes = this->get_output_buffer_length() * sizeof(sample);
    memset(this->out[0] + previous_overflow, 0, buffer_size_bytes - previous_overflow_bytes);

    if (previous_overflow > this->get_output_buffer_length())
    {
        printf("Runtime error (fft size %d, previous overflow %d)\n", this->fft_size, previous_overflow);
        throw std::runtime_error("IFFT: Moving overlapped segments from previous IFFT output would exceed memory bounds");
    }

    /*------------------------------------------------------------------------
     *
     *-----------------------------------------------------------------------*/
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    /*------------------------------------------------------------------------
     * Perform repeated inverse FFT, moving forward hop_size frames per
     * hop.
     *-----------------------------------------------------------------------*/
    for (int hop = 0; hop < this->num_hops; hop++)
    {
        float scale_factor = (float) hop_size / fft_size;
        scale_factor = (float) num_frames / fft_size;
        this->ifft(this->input->out[hop],
                   this->out[0] + (hop * hop_size),
                   true,
                   this->do_window,
                   scale_factor);
    }

    if (out != this->out)
    {
        memcpy(out[0], this->out[0], num_frames * sizeof(sample));
    }
}

}

#endif
