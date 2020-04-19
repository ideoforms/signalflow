#include "signal/node/operators/subtract.h"

namespace libsignal
{

Subtract::Subtract(NodeRef a, NodeRef b) : BinaryOpNode(a, b)
{
    this->name = "subtract";
}

void Subtract::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = input0->out[channel][frame] - input1->out[channel][frame];
        }
    }
}

}
