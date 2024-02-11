#include "signalflow/core/random.h"
#include "signalflow/node/fft/fft-magnitude-phase-array.h"

#include <algorithm>

namespace signalflow
{

FFTMagnitudePhaseArray::FFTMagnitudePhaseArray(NodeRef input,
                                               std::vector<float> magnitudes,
                                               std::vector<float> phases)
    : FFTOpNode(input)
{
    this->name = "fft-magnitude-phase-array";
    for (int i = 0; i < this->num_bins; i++)
    {
        this->magnitudes[0][i] = magnitudes[i];
        this->phases[0][i] = phases[i];
    }
}

void FFTMagnitudePhaseArray::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        // Should this be frame < num_bins * 2 (to account for dc/nyquist)?
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                out[hop][frame] = this->magnitudes[0][frame];
            }
            else
            {
                out[hop][frame] = this->phases[0][frame - this->num_bins];
                //                out[hop][frame] = random_uniform(-M_PI, M_PI);
            }
        }
    }
}

void FFTMagnitudePhaseArray::set_magnitudes(std::vector<float> &magnitudes)
{
    for (int bin = 0; bin < this->num_bins; bin++)
    {
        this->magnitudes[0][bin] = magnitudes[bin];
    }
}

void FFTMagnitudePhaseArray::set_phases(std::vector<float> &phases)
{
    for (int bin = 0; bin < this->num_bins; bin++)
    {
        this->phases[0][bin] = phases[bin];
    }
}

}
