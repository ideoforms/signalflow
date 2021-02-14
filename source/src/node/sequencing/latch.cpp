#include "signalflow/node/sequencing/latch.h"

namespace signalflow
{

Latch::Latch(NodeRef set, NodeRef reset)
    : set(set), reset(reset)
{
    this->name = "latch";

    this->create_input("set", this->set);
    this->create_input("reset", this->reset);

    this->alloc();
}

void Latch::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void Latch::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int i = 0; i < this->num_output_channels; i++)
        {
            this->value[i] = 1;
        }
    }
    else if (name == SIGNALFLOW_TRIGGER_RESET)
    {
        for (int i = 0; i < this->num_output_channels; i++)
        {
            this->value[i] = 0;
        }
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

void Latch::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->set, channel, frame))
            {
                this->value[channel] = 1;
            }
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->reset, channel, frame))
            {
                this->value[channel] = 0;
            }

            this->out[channel][frame] = (int) this->value[channel];
        }
    }
}

}
