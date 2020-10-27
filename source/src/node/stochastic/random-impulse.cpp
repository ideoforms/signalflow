#include "signalflow/node/stochastic/random-impulse.h"

#include "stdlib.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

namespace signalflow
{

RandomImpulse::RandomImpulse(NodeRef frequency, signalflow_event_distribution_t distribution)
    : frequency(frequency), distribution(distribution)
{
    this->name = "random-impulse";
    this->create_input("frequency", this->frequency);
    this->alloc();
}

void RandomImpulse::alloc()
{
    this->steps_remaining.resize(this->num_output_channels_allocated);
}

void RandomImpulse::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH()

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float freq = this->frequency->out[channel][frame];
            if (freq == 0)
            {
                out[channel][frame] = 0.0;
            }
            else
            {
                if (this->steps_remaining[channel] <= 0)
                {
                    if (this->distribution == SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM)
                    {
                        this->steps_remaining[channel] = random_integer(0, this->graph->get_sample_rate() / (freq / 2.0));
                    }
                    else if (this->distribution == SIGNALFLOW_EVENT_DISTRIBUTION_POISSON)
                    {
                        this->steps_remaining[channel] = this->graph->get_sample_rate() * -logf(1.0 - random_uniform()) / freq;
                    }
                }
                this->steps_remaining[channel]--;

                out[channel][frame] = (this->steps_remaining[channel] == 0) ? 1 : 0;
            }
        }
    }
}

}
