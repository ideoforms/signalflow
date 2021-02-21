#include "signalflow/node/stochastic/random-exponential.h"

#include <limits>

namespace signalflow
{

RandomExponential::RandomExponential(NodeRef min, NodeRef max, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), min(min), max(max), clock(clock)
{
    this->name = "random-exponential";
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomExponential::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
}

void RandomExponential::trigger(std::string name, float value)
{
    for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
    {
        this->value[channel] = signalflow_scale_lin_exp(this->random_uniform(0, 1), 0, 1, min->out[channel][0], this->max->out[channel][0]);
    }
}

void RandomExponential::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        if (this->value[channel] == std::numeric_limits<float>::max())
        {
            this->value[channel] = this->random_uniform(min->out[channel][0], this->max->out[channel][0]);
        }

        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = signalflow_scale_lin_exp(this->random_uniform(0, 1), 0, 1, min->out[channel][frame], this->max->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
