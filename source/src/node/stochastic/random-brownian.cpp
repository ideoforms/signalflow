#include "signalflow/node/stochastic/random-brownian.h"

namespace signalflow
{

RandomBrownian::RandomBrownian(NodeRef min, NodeRef max, NodeRef delta, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), min(min), max(max), delta(delta), clock(clock)
{
    this->name = "random-brownian";
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->create_input("delta", this->delta);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomBrownian::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void RandomBrownian::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
        {
            this->value[channel] += this->random_gaussian(0, this->delta->out[channel][0]);

            if (this->value[channel] > this->max->out[channel][0])
                this->value[channel] = this->max->out[channel][0] - (this->value[channel] - this->max->out[channel][0]);
            else if (this->value[channel] < this->min->out[channel][0])
                this->value[channel] = this->min->out[channel][0] + (this->min->out[channel][0] - this->value[channel]);
        }
    }
    else
    {
        this->StochasticNode::trigger(name, value);
    }
}

void RandomBrownian::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (clock == nullptr || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] += this->random_gaussian(0, this->delta->out[channel][frame]);
                if (this->value[channel] > this->max->out[channel][frame])
                    this->value[channel] = this->max->out[channel][frame] - (this->value[channel] - this->max->out[channel][frame]);
                else if (this->value[channel] < this->min->out[channel][frame])
                    this->value[channel] = this->min->out[channel][frame] + (this->min->out[channel][frame] - this->value[channel]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
