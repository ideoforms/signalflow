#include "signalflow/node/stochastic/random-coin.h"

#include "signalflow/core/random.h"

#include <limits>

namespace signalflow
{

RandomCoin::RandomCoin(NodeRef probability, NodeRef clock)
    : probability(probability), clock(clock)
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
    for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
    {
        this->value[channel] = random_coin(this->probability->out[channel][0]);
    }
}

void RandomCoin::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        if (this->value[channel] == std::numeric_limits<float>::max())
        {
            this->value[channel] = random_coin(this->probability->out[channel][0]);
        }

        for (int frame = 0; frame < num_frames; frame++)
        {
            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = random_coin(this->probability->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
