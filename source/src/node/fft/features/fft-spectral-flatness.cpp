#include "signalflow/node/fft/features/fft-spectral-flatness.h"
#include <algorithm>

namespace signalflow
{

FFTSpectralFlatness::FFTSpectralFlatness(NodeRef input)
    : FFTOpNode(input)
{
    this->name = "fft-spectral-flatness";
}

void FFTSpectralFlatness::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        double numerator = 0.0;
        double denominator = 0.0;
        int count = 0;
        float rv = 0.0;

        for (int frame = 0; frame < this->num_bins; frame++)
        {
            if (this->input->out[hop][frame] > 0.0)
            {
                numerator += log(this->input->out[hop][frame]);
                denominator += this->input->out[hop][frame];
                count++;
            }
        }

        if (count)
        {
            numerator = exp(numerator / (double) count);
            denominator = denominator / (double) count;
            rv = numerator / denominator;
        }

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            out[hop][frame] = rv;
        }
    }
}

}
