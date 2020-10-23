#include "signalflow/node/operators/subtract.h"

namespace signalflow
{

Subtract::Subtract(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "subtract";
}

void Subtract::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
#ifdef __APPLE__
        vDSP_vsub(input1->out[channel], 1, input0->out[channel], 1, out[channel], 1, num_frames);
#else
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = input0->out[channel][frame] - input1->out[channel][frame];
        }
#endif
    }
}

}
