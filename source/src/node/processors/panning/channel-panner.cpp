#include "signalflow/node/processors/panning/channel-panner.h"

namespace signalflow
{

ChannelPanner::ChannelPanner(int channels, NodeRef input, NodeRef pan, NodeRef width)
    : input(input), pan(pan), width(width)
{
    this->name = "channel-panner";

    this->set_channels(1, channels);

    this->create_input("input", this->input);
    this->create_input("pan", this->pan);
    this->create_input("width", this->width);

    // can this now be done in the same way as create_input?
    this->create_property("num_channels", this->num_channels);
    this->set_property("num_channels", channels);
}

void ChannelPanner::set_property(std::string name, const PropertyRef &value)
{
    if (name == "num_channels")
    {
        this->set_channels(1, value->int_value());
    }

    Node::set_property(name, value);
}

void ChannelPanner::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pan = this->pan->out[0][frame];
        sample in = this->input->out[0][frame];
        sample width = this->width->out[0][frame];

        for (int channel = 0; channel < this->get_num_output_channels(); channel++)
        {
            float distance = fabsf(pan - channel);
            if (distance < width)
            {
                float amp = 1.0f - distance / width;
                out[channel][frame] = in * amp;
            }
            else
            {
                out[channel][frame] = 0.0;
            }
        }
    }
}

}
