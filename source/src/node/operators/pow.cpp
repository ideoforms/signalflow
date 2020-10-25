#include "signalflow/node/operators/pow.h"

namespace signalflow
{

Pow::Pow(NodeRef a, NodeRef b)
    : BinaryOpNode(a, b)
{
    this->name = "pow";
}

void Pow::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
#ifdef __APPLE__
        vvpowf(out[channel], input1->out[channel], input0->out[channel], &num_frames);
#else
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = powf(input0->out[channel][frame], input1->out[channel][frame]);
        }
#endif
    }
}

}
