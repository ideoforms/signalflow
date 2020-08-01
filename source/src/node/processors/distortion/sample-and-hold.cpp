#include "signalflow/node/processors/distortion/sample-and-hold.h"

namespace signalflow
{

SampleAndHold::SampleAndHold(NodeRef input, NodeRef clock)
    : UnaryOpNode(input), clock(clock)
{
    this->name = "sample-and-hold";
    this->create_input("clock", this->clock);
    this->values.resize(SIGNALFLOW_MAX_CHANNELS);
    this->values.clear();
}

void SampleAndHold::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_TRIGGER(this->clock, frame))
            {
                for (int channel = 0; channel < this->num_output_channels; channel++)
                {
                    values[channel] = input->out[channel][frame];
                }
            }
            out[channel][frame] = values[channel];
        }
    }
}

void SampleAndHold::trigger(std::string name, float value)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        values[channel] = input->out[channel][0];
    }
}

}
