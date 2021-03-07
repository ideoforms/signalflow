#include "signalflow/node/sequencing/counter.h"

#include <limits>

namespace signalflow
{

Counter::Counter(NodeRef clock, NodeRef min, NodeRef max)
    : Node(), clock(clock), min(min), max(max)
{
    this->name = "counter";

    this->create_input("clock", this->clock);
    this->create_input("min", this->min);
    this->create_input("max", this->max);

    this->alloc();
}

void Counter::alloc()
{
    this->counter.resize(this->num_output_channels_allocated, std::numeric_limits<int>::max());
}

void Counter::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (this->counter[channel] == std::numeric_limits<int>::max())
            {
                this->counter[channel] = this->min->out[channel][0];
            }
            else
            {
                this->counter[channel] += 1;
                if (this->counter[channel] >= this->max->out[channel][0])
                    this->counter[channel] = this->min->out[channel][0];
            }
        }
    }
}

void Counter::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            bool rv = SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame);
            if (rv)
            {
                if (this->counter[channel] == std::numeric_limits<int>::max())
                {
                    this->counter[channel] = this->min->out[channel][frame];
                }
                else
                {
                    this->counter[channel] += 1;
                    if (this->counter[channel] >= this->max->out[channel][frame])
                        this->counter[channel] = this->min->out[channel][frame];
                }
            }
            if (this->counter[channel] == std::numeric_limits<int>::max())
            {
                out[channel][frame] = 0;
            }
            else
            {
                out[channel][frame] = this->counter[channel];
            }
        }
    }
}

}
