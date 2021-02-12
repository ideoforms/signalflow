#include "signalflow/core/util.h"
#include "signalflow/node/processors/wrap.h"

namespace signalflow
{

Wrap::Wrap(NodeRef input, NodeRef min, NodeRef max)
    : UnaryOpNode(input), min(min), max(max)
{
    this->name = "wrap";
    this->create_input("min", this->min);
    this->create_input("max", this->max);
}

void Wrap::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = signalflow_wrap(input->out[channel][frame],
                                                  this->min->out[channel][frame],
                                                  this->max->out[channel][frame]);
        }
    }
}

}
