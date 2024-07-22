#include "signalflow/node/fft/fft-cross-fade.h"
#include <algorithm>

namespace signalflow
{

FFTCrossFade::FFTCrossFade(NodeRef inputA, NodeRef inputB, NodeRef crossfade)
    : FFTOpNode(inputA), inputB(inputB), crossfade(crossfade)
{
    this->name = "fft-contrast";

    this->create_input("inputB", this->inputB);
    this->create_input("crossfade", this->crossfade);
}

void FFTCrossFade::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        float crossfade = this->crossfade->out[0][0];

        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                out[hop][frame] = (input->out[hop][frame] * (1 - crossfade)) + (inputB->out[hop][frame] * (crossfade));
            }
            else
            {
                out[hop][frame] = (input->out[hop][frame] * (1 - crossfade)) + (inputB->out[hop][frame] * (crossfade));
            }
        }
    }
}

}
