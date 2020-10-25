#include "signalflow/node/operators/add.h"

namespace signalflow
{

Add::Add(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "add";
}

void Add::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
#ifdef __APPLE__
        vDSP_vadd(input0->out[channel], 1, input1->out[channel], 1, out[channel], 1, num_frames);
#else
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = input0->out[channel][frame] + input1->out[channel][frame];
        }
#endif
    }
}

}
