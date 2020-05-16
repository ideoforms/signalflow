#ifdef __APPLE__

#include "signal/node/fft/convolve.h"
#include "signal/node/fft/fft.h"

namespace libsignal
{

FFTConvolve::FFTConvolve(NodeRef input, BufferRef buffer)
    : FFTOpNode(input), buffer(buffer)
{
    this->name = "fft-convolve";

    FFT *fft = new FFT();
    this->fft_buffer = new sample[this->fft_size]();
    fft->fft(this->buffer->data[0], this->fft_buffer);
    delete fft;

    this->add_buffer("buffer", this->buffer);
}

void FFTConvolve::process(sample **out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/
        for (int frame = 0; frame < this->fft_size; frame++)
        {
                if (frame < this->num_bins)
                {
                    /*------------------------------------------------------------------------
                     * Magnitudes
                     *-----------------------------------------------------------------------*/
                    this->out[hop][frame] = this->input->out[hop][frame] * this->fft_buffer[frame];
                }
                else
                {
                    /*------------------------------------------------------------------------
                     * Phases
                     *-----------------------------------------------------------------------*/
                    this->out[hop][frame] = this->input->out[hop][frame] + this->fft_buffer[frame];
                }
        }
    }
}

}

#endif