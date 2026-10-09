#include "signalflow/node/fft/processors/fft-tonality.h"

#include <algorithm>
#include <array>

namespace signalflow
{

FFTTonality::FFTTonality(NodeRef input, NodeRef level, NodeRef smoothing)
    : FFTOpNode(input), level(level), smoothing(smoothing)
{
    this->name = "fft-tonality";
    this->create_input("level", this->level);
    this->create_input("smoothing", this->smoothing);
}

void FFTTonality::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        /*------------------------------------------------------------------------
         * Rather than num_frames here, we need to iterate over fft_size frames
         *  - as each block contains a whole fft of samples.
         *-----------------------------------------------------------------------*/

        memcpy(this->mags_smoothed, input->out[hop], num_bins * sizeof(float));
        float smoothing = this->smoothing->out[0][0];
        float one_minus_smoothing = 1.0 - smoothing;
        for (int bin_index = 1; bin_index < num_bins; bin_index++)
        {
            this->mags_smoothed[bin_index] = (this->mags_smoothed[bin_index - 1] * smoothing) + (this->mags_smoothed[bin_index] * one_minus_smoothing);
        }
        for (int bin_index = num_bins - 2; bin_index >= 0; bin_index--)
        {
            this->mags_smoothed[bin_index] = (this->mags_smoothed[bin_index + 1] * smoothing) + (this->mags_smoothed[bin_index] * one_minus_smoothing);
        }

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                out[hop][frame] = input->out[hop][frame] - this->level->out[0][0] * this->mags_smoothed[frame];
                if (out[hop][frame] < 0)
                    out[hop][frame] = 0;
            }
            else
            {
                out[hop][frame] = input->out[hop][frame];
            }
        }
    }
}

}
