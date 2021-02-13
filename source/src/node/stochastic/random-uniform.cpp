#include "signalflow/node/stochastic/random-uniform.h"

#include <limits>

namespace signalflow
{

RandomUniform::RandomUniform(NodeRef min, NodeRef max, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), min(min), max(max), clock(clock)
{
    this->name = "random-uniform";
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomUniform::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
}

void RandomUniform::trigger(std::string name, float value)
{
    for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
    {
        this->value[channel] = this->random_uniform(min->out[channel][0], this->max->out[channel][0]);
    }
}

void RandomUniform::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        if (this->value[channel] == std::numeric_limits<float>::max())
        {
            this->value[channel] = this->random_uniform(min->out[channel][0], this->max->out[channel][0]);
        }

        for (int frame = 0; frame < num_frames; frame++)
        {
            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = this->random_uniform(min->out[channel][frame], this->max->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
