#include "signalflow/node/processors/distortion/sample-and-hold.h"

namespace signalflow
{

SampleAndHold::SampleAndHold(NodeRef input, NodeRef clock)
    : UnaryOpNode(input), clock(clock)
{
    this->name = "sample-and-hold";
    this->create_input("clock", this->clock);
    this->alloc();
}

void SampleAndHold::alloc()
{
    this->values.resize(this->num_output_channels_allocated);
}

void SampleAndHold::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame))
            {
                values[channel] = input->out[channel][frame];
            }
            out[channel][frame] = values[channel];
        }
    }
}

void SampleAndHold::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            values[channel] = input->out[channel][0];
        }
    }
}

}
