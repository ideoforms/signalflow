#include "signalflow/node/processors/panning/azimuth-panner.h"

namespace signalflow
{

AzimuthPanner::AzimuthPanner(int num_channels, NodeRef input, NodeRef pan, NodeRef width)
    : num_channels(num_channels), input(input), pan(pan), width(width)
{
    this->name = "azimuth-panner";

    this->create_property("num_channels", this->num_channels);
    this->create_input("input", this->input);
    this->create_input("pan", this->pan);
    this->create_input("width", this->width);

    this->set_channels(1, num_channels);
}

void AzimuthPanner::set_property(std::string property_name, const PropertyRef &value)
{
    if (property_name == "num_channels")
    {
        this->set_channels(1, value->int_value());
    }

    Node::set_property(property_name, value);
}

void AzimuthPanner::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pan = this->pan->out[0][frame] * this->get_num_output_channels() / 2;
        sample in = this->input->out[0][frame];
        sample width = this->width->out[0][frame];

        for (int channel = 0; channel < this->get_num_output_channels(); channel++)
        {
            float distance = fabsf(pan + 0.5f - channel);
            distance = signalflow_fold(distance,
                                       0,
                                       this->get_num_output_channels() / 2);
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
