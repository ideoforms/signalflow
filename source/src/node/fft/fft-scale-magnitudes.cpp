#include "signalflow/core/random.h"
#include "signalflow/node/fft/fft-scale-magnitudes.h"

#include <algorithm>

namespace signalflow
{

FFTScaleMagnitudes::FFTScaleMagnitudes(NodeRef input,
                                       std::vector<float> scale)
    : FFTOpNode(input), scale(scale)
{
    this->name = "fft-scale-magnitudes";
    if (this->scale.size() != this->num_bins)
    {
        throw std::runtime_error("FFTScaleMagnitudes: scale array must be same length as magnitude array (" + std::to_string(this->num_bins) + ")");
    }
}

void FFTScaleMagnitudes::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                // magnitudes
                out[hop][frame] = this->input->out[hop][frame] * this->scale[frame];
            }
            else
            {
                // phases
                out[hop][frame] = this->input->out[hop][frame];
            }
        }
    }
}

}
