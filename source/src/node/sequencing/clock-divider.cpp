#include "signalflow/node/sequencing/clock-divider.h"

namespace signalflow
{

ClockDivider::ClockDivider(NodeRef clock, NodeRef factor)
    : Node(), clock(clock), factor(factor)
{
    this->name = "clock-divider";

    this->create_input("clock", this->clock);
    this->create_input("factor", this->factor);

    this->alloc();
}

void ClockDivider::alloc()
{
    this->counter.resize(this->num_output_channels_allocated);
}

void ClockDivider::trigger(std::string name, float value)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        this->counter[channel] += 1;
    }
}

void ClockDivider::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                int factor = this->factor->out[channel][frame];
                if (this->counter[channel] >= factor)
                {
                    this->counter[channel] = 0;
                    this->out[channel][frame] = 1;
                }
                else
                {
                    this->out[channel][frame] = 0;
                }
                this->counter[channel] += 1;
            }
            else
            {
                this->out[channel][frame] = 0;
            }
        }
    }
}

}
