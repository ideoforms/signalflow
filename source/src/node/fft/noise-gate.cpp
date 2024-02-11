#include "signalflow/node/fft/noise-gate.h"

#include <algorithm>

namespace signalflow
{

FFTNoiseGate::FFTNoiseGate(NodeRef input, NodeRef threshold, NodeRef invert)
    : FFTOpNode(input), threshold(threshold), invert(invert)
{
    this->name = "fft_noise_gate";
    this->create_input("threshold", this->threshold);
    this->create_input("invert", this->invert);
}

void FFTNoiseGate::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/
        memcpy(this->mags, &input->out[hop][0], sizeof(float) * num_bins);
        std::sort(this->mags, mags + num_bins);
        float cutoff = this->mags[(int) (this->threshold->out[0][0] * num_bins)];
        bool invert = (bool) this->invert->out[0][0];

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                if ((!invert && (input->out[hop][frame] > cutoff)) || (invert && (input->out[hop][frame] < cutoff)))
                    out[hop][frame] = input->out[hop][frame];
                else
                    out[hop][frame] = 0.0;
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }
    }
}

}
