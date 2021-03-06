#include "signalflow/node/stochastic/random-coin.h"

#include <limits>

namespace signalflow
{

RandomCoin::RandomCoin(NodeRef probability, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), probability(probability), clock(clock)
{
    this->name = "random-coin";
    this->create_input("probability", this->probability);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomCoin::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
}

void RandomCoin::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
        {
            this->value[channel] = this->random_uniform() < this->probability->out[channel][0];
        }
    }
    else
    {
        this->StochasticNode::trigger(name, value);
    }
}

void RandomCoin::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (this->value[channel] == std::numeric_limits<float>::max() || clock == nullptr || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = this->random_uniform() < this->probability->out[channel][frame];
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
