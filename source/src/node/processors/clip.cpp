#include "signalflow/node/processors/clip.h"

namespace signalflow
{

Clip::Clip(NodeRef input, NodeRef min, NodeRef max)
    : UnaryOpNode(input), min(min), max(max)
{
    this->name = "clip";
    this->create_input("min", this->min);
    this->create_input("max", this->max);
}

void Clip::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = signalflow_clip(input->out[channel][frame],
                                                  this->min->out[channel][frame],
                                                  this->max->out[channel][frame]);
        }
    }
}

}
