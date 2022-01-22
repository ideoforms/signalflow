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

    this->create_property("offset", this->offset);
    this->create_property("maximum", this->maximum);
    this->create_property("step", this->step);

    this->set_channels(this->input->get_num_output_channels(),
                       ceilf((this->maximum->int_value() - this->offset->int_value()) / this->step->int_value()));
}

void ChannelSelect::process(Buffer &out, int num_frames)
{
    int output_channel = 0;
    for (int channel = this->offset->int_value();
         channel < this->maximum->int_value();
         channel += this->step->int_value())
    {
        memcpy(out[output_channel], input->out[channel], num_frames * sizeof(sample));
        output_channel++;
    }
}

}
