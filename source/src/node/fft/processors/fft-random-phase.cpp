#include "signalflow/core/random.h"
#include "signalflow/node/fft/processors/fft-random-phase.h"

#include <algorithm>

namespace signalflow
{

FFTRandomPhase::FFTRandomPhase(NodeRef input, NodeRef level)
    : FFTOpNode(input), level(level)
{
    this->name = "fft-random-phase";
    this->create_input("level", this->level);
}

void FFTRandomPhase::process(Buffer &out, int num_frames)
{
    FFTNode *fftnode = (FFTNode *) this->input.get();
    this->num_hops = fftnode->num_hops;
    float level = this->level->out[0][0];

    for (int hop = 0; hop < this->num_hops; hop++)
    {
        // Should this be frame < num_bins * 2 (to account for dc/nyquist)?
        for (int frame = 0; frame < this->fft_size; frame++)
        {
            if (frame < this->num_bins)
            {
                out[hop][frame] = this->input->out[hop][frame];
            }
            else
            {
                if (level == 1.0)
                {
                    out[hop][frame] = random_uniform(-M_PI, M_PI);
                }
                else
                {
                    out[hop][frame] = this->input->out[hop][frame] + random_uniform(-M_PI * level, M_PI * level);
                    if (out[hop][frame] >= M_PI)
                        out[hop][frame] -= M_PI * 2;
                    if (out[hop][frame] <= -M_PI)
                        out[hop][frame] += M_PI * 2;
                }
            }
        }
    }
}

}
