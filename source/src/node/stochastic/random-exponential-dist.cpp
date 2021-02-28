#include "signalflow/core/random.h"
#include "signalflow/node/stochastic/random-exponential-dist.h"

namespace signalflow
{

RandomExponentialDist::RandomExponentialDist(NodeRef scale, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), scale(scale), clock(clock)
{
    this->name = "random-exponential-dist";
    this->create_input("scale", this->scale);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomExponentialDist::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void RandomExponentialDist::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
        {
            this->value[channel] = this->random_exponential(this->scale->out[channel][0]);
        }
    }
    else
    {
        this->StochasticNode::trigger(name, value);
    }
}

void RandomExponentialDist::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = this->random_exponential(this->scale->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
