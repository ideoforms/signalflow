#include "signalflow/core/random.h"
#include "signalflow/node/stochastic/random-exponential.h"

namespace signalflow
{

RandomExponential::RandomExponential(NodeRef scale, NodeRef clock)
    : scale(scale), clock(clock)
{
    this->name = "random-exponential";
    this->create_input("scale", this->scale);
    this->create_input("clock", this->clock);
    this->alloc();
    this->trigger();
}

void RandomExponential::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
}

void RandomExponential::trigger(std::string name, float value)
{
    for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
    {
        this->value[channel] = random_exponential(this->scale->out[channel][0]);
    }
}

void RandomExponential::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->value[channel] = random_exponential(this->scale->out[channel][frame]);
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
