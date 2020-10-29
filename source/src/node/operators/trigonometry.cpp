#include "signalflow/node/operators/trigonometry.h"
#include <math.h>

namespace signalflow
{

Tanh::Tanh(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "tanh";
}

void Tanh::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = tanhf(this->input->out[channel][frame]);
        }
    }
}

}
