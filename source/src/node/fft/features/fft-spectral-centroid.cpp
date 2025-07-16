#include "signalflow/core/graph.h"
#include "signalflow/node/fft/features/fft-spectral-centroid.h"
#include <algorithm>

namespace signalflow
{

FFTSpectralCentroid::FFTSpectralCentroid(NodeRef input)
    : FFTOpNode(input)
{
    this->name = "fft-spectral-centroid";
}

void FFTSpectralCentroid::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        double numerator = 0.0;
        double denominator = 0.0;
        float rv = 0.0;

        for (int frame = 1; frame < this->num_bins; frame++)
        {
            if (this->input->out[hop][frame] > 1e-12)
            {
                double frequency = (this->graph->get_sample_rate() * frame) / (2.0 * (num_bins - 1));
                numerator += frequency * this->input->out[hop][frame];
                denominator += this->input->out[hop][frame];
            }
        }

        if (denominator > 1e-12)
        {
            rv = numerator / denominator;
        }

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            out[hop][frame] = rv;
        }
    }
}

}
