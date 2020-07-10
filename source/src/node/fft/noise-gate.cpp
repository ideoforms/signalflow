#include "signalflow/node/fft/noise-gate.h"

#include <algorithm>
#include <array>

namespace signalflow
{

FFTNoiseGate::FFTNoiseGate(NodeRef input, NodeRef threshold)
    : FFTOpNode(input), threshold(threshold)
{
    this->name = "fft_noise_gate";
    this->add_input("threshold", this->threshold);
}

void FFTNoiseGate::process(sample **out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/
        float mags[num_bins];
        memcpy(mags, &input->out[hop][0], sizeof(float) * num_bins);
        std::sort(mags, mags + num_bins);
        float cutoff = mags[(int) (this->threshold->out[0][0] * num_bins)];

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                if (input->out[hop][frame] > cutoff)
                    out[hop][frame] = input->out[hop][frame];
                else
                    out[hop][frame] = 0.0;
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }

        /*
        float min_magnitude = 1e6;
        float max_magnitude = 0.0;
        for (int bin = 0; bin < this->num_bins; bin++)
        {
            if (this->out[hop][bin] < min_magnitude)
                min_magnitude = input->out[hop][bin];
            if (this->out[hop][bin] > max_magnitude)
                max_magnitude = input->out[hop][bin];
        }

        float cutoff = min_magnitude * powf(max_magnitude / min_magnitude, this->threshold->out[0][0]);

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                if (input->out[hop][frame] > cutoff)
                    out[hop][frame] = input->out[hop][frame];
                else
                    out[hop][frame] = 0.0;
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }
        */
    }
}

}
