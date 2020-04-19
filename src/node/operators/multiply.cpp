#include "signal/node/operators/multiply.h"

namespace libsignal
{

Multiply::Multiply(NodeRef a, NodeRef b) : BinaryOpNode(a, b)
{
    this->name = "multiply";
}

void Multiply::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        #ifdef __APPLE__

            vDSP_vmul(input0->out[channel], 1, input1->out[channel], 1, out[channel], 1, num_frames);

        #else

            for (int frame = 0; frame < num_frames; frame++)
                out[channel][frame] = input0->out[channel][frame] * input1->out[channel][frame];

        #endif
    }
}

}

