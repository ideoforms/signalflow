#include "signalflow/node/stochastic/random-choice.h"

#include <limits>

namespace signalflow
{

RandomChoice::RandomChoice(std::vector<float> values, NodeRef clock, NodeRef reset)
    : StochasticNode(reset), values(values), clock(clock)
{
    this->name = "random-choice";
    this->create_input("clock", this->clock);
    this->alloc();
}

void RandomChoice::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
}

void RandomChoice::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels_allocated; channel++)
        {
            int index = (int) this->random_uniform(0, this->values.size());
            this->value[channel] = this->values[index];
        }
    }
}

void RandomChoice::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            if (clock == 0 || SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                int index = (int) this->random_uniform(0, this->values.size());
                this->value[channel] = this->values[index];
            }

            this->out[channel][frame] = this->value[channel];
        }
    }
}

}
