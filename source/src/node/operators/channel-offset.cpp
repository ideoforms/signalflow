#include "signalflow/node/operators/channel-offset.h"

namespace signalflow
{

ChannelOffset::ChannelOffset(int offset, NodeRef input)
    : UnaryOpNode(input), offset(offset)
{
    if (!input)
    {
        throw std::runtime_error("ChannelOffset: No input specified");
    }

    this->name = "channel-offset";

    this->create_property("offset", this->offset);

    this->set_channels(this->input->get_num_output_channels(),
                       this->input->get_num_output_channels() + this->offset->int_value());
}

void ChannelOffset::process(Buffer &out, int num_frames)
{
    int output_channel = 0;
    for (int channel = 0; channel < this->offset->int_value(); channel++)
    {
        memset(out[output_channel], 0, num_frames * sizeof(sample));
        output_channel++;
    }
    for (int channel = 0; channel < this->input->get_num_output_channels(); channel++)
    {
        memcpy(out[output_channel], this->input->out[channel], num_frames * sizeof(sample));
        output_channel++;
    }
}

}
