#include "signalflow/node/sequencing/counter.h"

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
    this->counter.resize(this->num_output_channels_allocated);
}

void Counter::trigger(std::string name, float value)
{
    for (int i = 0; i < this->num_output_channels; i++)
    {
        this->counter[i] += 1;
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
                this->counter[channel] += 1;
                if (this->counter[channel] >= this->max->out[channel][frame])
                    this->counter[channel] = this->min->out[channel][frame];
            }
            this->out[channel][frame] = this->counter[channel];
        }
    }
}

}
