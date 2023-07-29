#include "signalflow/node/processors/smooth.h"

namespace signalflow
{

Smooth::Smooth(NodeRef input, NodeRef smooth)
    : UnaryOpNode(input), smooth(smooth)
{
    this->name = "smooth";
    this->create_input("smooth", this->smooth);
    this->alloc();
}

void Smooth::alloc()
{
    this->values.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
}

void Smooth::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * On first step, jump immediately to value.
     *-------------------------------------------------------------------------------*/
    if (values[0] == std::numeric_limits<float>::max())
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            values[channel] = input->out[channel][0];
        }
    }

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            values[channel] = (smooth->out[channel][frame] * values[channel]) + ((1.0 - smooth->out[channel][frame]) * input->out[channel][frame]);
            out[channel][frame] = values[channel];
        }
    }
}

}
