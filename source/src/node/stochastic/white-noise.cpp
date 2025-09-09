#include "signalflow/node/stochastic/white-noise.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

#include <limits>
#include <stdlib.h>

namespace signalflow
{

WhiteNoise::WhiteNoise(NodeRef frequency,
                       NodeRef min, NodeRef max,
                       bool interpolate, bool random_interval,
                       NodeRef reset)
    : StochasticNode(reset), frequency(frequency), min(min), max(max), interpolate(interpolate), random_interval(random_interval)
{
    this->name = "white-noise";
    this->create_input("frequency", this->frequency);
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->alloc();
}

void WhiteNoise::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
    this->steps_remaining.resize(this->num_output_channels_allocated);
    this->step_change.resize(this->num_output_channels_allocated);
}

void WhiteNoise::process(Buffer &out, int num_frames)
{
    // Local deferences of input NodeRefs save about 5% cycles for this node
    Node *min = this->min.get();
    Node *max = this->max.get();
    Node *frequency = this->frequency.get();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        if (this->value[channel] == std::numeric_limits<float>::max())
        {
            // TODO: Put this in an init block that is available to all
            // nodes on their first block?
            this->value[channel] = this->min->out[0][0];
        }

        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            float vmin = min->out[channel][frame];
            float vmax = max->out[channel][frame];
            float vfrequency = frequency->out[channel][frame];
            if (!vfrequency)
                vfrequency = this->graph->get_sample_rate();

            if (this->steps_remaining[channel] <= 0)
            {
                // pick a new target value
                float target = this->random_uniform(vmin, vmax);

                if (vfrequency > 0)
                {
                    if (random_interval)
                    {
                        this->steps_remaining[channel] = (int) (this->random_uniform() * this->graph->get_sample_rate() / (vfrequency / 2.0));
                    }
                    else
                    {
                        this->steps_remaining[channel] = (int) round(this->graph->get_sample_rate() / vfrequency);
                    }
                    if (this->steps_remaining[channel] == 0)
                        this->steps_remaining[channel] = 1;
                    this->step_change[channel] = (target - this->value[channel]) / this->steps_remaining[channel];
                }
                else
                {
                    this->steps_remaining[channel] = 0;
                    this->step_change[channel] = target - this->value[channel];
                }

                if (!this->interpolate)
                {
                    this->value[channel] = target;
                    this->step_change[channel] = 0;
                }
            }

            this->value[channel] += this->step_change[channel];

            out[channel][frame] = this->value[channel];

            this->steps_remaining[channel]--;
        }
    }
}

}
