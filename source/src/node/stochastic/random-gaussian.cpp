#include "signalflow/core/random.h"
#include "signalflow/node/stochastic/random-gaussian.h"

namespace signalflow
{

RandomGaussian::RandomGaussian(NodeRef mean, NodeRef sigma, NodeRef clock)
    : mean(mean), sigma(sigma), clock(clock)
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
    for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
    {
        this->value[channel] = random_gaussian(this->mean->out[channel][0],
                                               this->sigma->out[channel][0]);
    }
}

void RandomGaussian::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = random_gaussian(this->mean->out[channel][frame],
                                                       this->sigma->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
