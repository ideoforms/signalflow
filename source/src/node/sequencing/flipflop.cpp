#include "signalflow/node/sequencing/flipflop.h"

namespace signalflow
{

FlipFlop::FlipFlop(NodeRef clock)
    : clock(clock)
{
    this->name = "counter";

    this->create_input("clock", this->clock);

    this->alloc();
}

void FlipFlop::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void FlipFlop::trigger(std::string name, float value)
{
    for (int i = 0; i < this->num_output_channels; i++)
    {
        this->value[i] = !this->value[i];
    }
}

void FlipFlop::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                this->value[channel] = !this->value[channel];
            }
            this->out[channel][frame] = (int) this->value[channel];
        }
    }
}

}
