#include "signalflow/node/operators/channel-select.h"

namespace signalflow
{

ChannelSelect::ChannelSelect(NodeRef input, int offset, int maximum, int step)
    : UnaryOpNode(input), offset(offset), maximum(maximum ? maximum : offset + 1), step(step)
{
    if (!input)
    {
        throw std::runtime_error("ChannelSelect: No input specified");
    }

    this->name = "channel-select";

    this->num_input_channels = this->input->num_output_channels;
    this->num_output_channels = (int) ceilf((this->offset + this->maximum) / this->step);
}

void ChannelSelect::process(sample **out, int num_frames)
{
    int output_channel = 0;
    for (int channel = this->offset; channel < this->maximum; channel += this->step)
    {
        memcpy(out[output_channel], input->out[channel], num_frames * sizeof(sample));
        output_channel++;
    }
}

}
