#include "signalflow/node/stochastic/random-gaussian.h"

namespace signalflow
{

RandomGaussian::RandomGaussian(NodeRef mean, NodeRef sigma, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), mean(mean), sigma(sigma), clock(clock)
{
    this->name = "random-gaussian";
    this->create_input("mean", this->mean);
    this->create_input("sigma", this->sigma);
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomGaussian::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void RandomGaussian::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
        {
            this->value[channel] = this->random_gaussian(this->mean->out[channel][0],
                                                         this->sigma->out[channel][0]);
        }
    }
    else
    {
        this->StochasticNode::trigger(name, value);
    }
}

void RandomGaussian::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (clock == nullptr || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = this->random_gaussian(this->mean->out[channel][frame],
                                                             this->sigma->out[channel][frame]);
            }

            out[channel][frame] = this->value[channel];
        }
    }
}

}
