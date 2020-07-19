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

    memset(this->counter, 0, sizeof(int) * SIGNAL_MAX_CHANNELS);
}

void Counter::trigger(std::string name, float value)
{
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        this->counter[i] += 1;
    }
}

void Counter::process(sample **out, int num_frames)
{
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        if (this->counter[i] >= this->max->out[0][0])
            this->counter[i] = this->min->out[0][0];
    }
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->counter[channel];
        }
    }
}

}
