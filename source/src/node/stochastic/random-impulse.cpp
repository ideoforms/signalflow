#include "signalflow/node/stochastic/random-impulse.h"

#include "stdlib.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

namespace signalflow
{

RandomImpulse::RandomImpulse(NodeRef frequency, signal_event_distribution_t distribution)
    : frequency(frequency), distribution(distribution)
{
    this->steps_remaining = 0;

    this->name = "random-impulse";
    this->create_input("frequency", this->frequency);
}

void RandomImpulse::process(sample **out, int num_frames)
{
    SIGNAL_CHECK_GRAPH()

    for (int frame = 0; frame < num_frames; frame++)
    {
        float freq = this->frequency->out[0][frame];
        if (freq == 0)
        {
            for (int channel = 0; channel < this->num_output_channels; channel++)
            {
                out[channel][frame] = 0.0;
            }
        }
        else
        {
            if (this->steps_remaining <= 0)
            {
                if (this->distribution == SIGNAL_EVENT_DISTRIBUTION_UNIFORM)
                {
                    this->steps_remaining = random_integer(0, this->graph->get_sample_rate() / (freq / 2.0));
                }
                else if (this->distribution == SIGNAL_EVENT_DISTRIBUTION_POISSON)
                {
                    this->steps_remaining = this->graph->get_sample_rate() * -logf(1.0 - random_uniform()) / freq;
                }
            }
            this->steps_remaining--;

            for (int channel = 0; channel < this->num_output_channels; channel++)
            {
                out[channel][frame] = (this->steps_remaining == 0) ? 1 : 0;
            }
        }
    }
}

}
