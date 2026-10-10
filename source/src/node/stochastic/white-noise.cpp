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
    int sample_rate = this->graph->get_sample_rate();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        if (this->value[channel] == std::numeric_limits<float>::max())
        {
            // TODO: Put this in an init block that is available to all
            // nodes on their first block?
            this->value[channel] = this->min->out[0][0];
        }

        /*--------------------------------------------------------------------------------
         * Keep per-channel state in local variables during the loop, to avoid loads
         * and stores per sample (see SineOscillator).
         *--------------------------------------------------------------------------------*/
        sample value = this->value[channel];
        int steps_remaining = this->steps_remaining[channel];
        float step_change = this->step_change[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNALFLOW_PROCESS_STOCHASTIC_NODE_RESET_TRIGGER()

            float vmin = min->out[channel][frame];
            float vmax = max->out[channel][frame];
            float vfrequency = frequency->out[channel][frame];
            if (!vfrequency)
                vfrequency = sample_rate;

            if (steps_remaining <= 0)
            {
                // pick a new target value
                float target = this->random_uniform(vmin, vmax);

                if (vfrequency > 0)
                {
                    if (random_interval)
                    {
                        steps_remaining = (int) (this->random_uniform() * sample_rate / (vfrequency / 2.0));
                    }
                    else
                    {
                        steps_remaining = (int) round(sample_rate / vfrequency);
                    }
                    if (steps_remaining == 0)
                        steps_remaining = 1;
                    step_change = (target - value) / steps_remaining;
                }
                else
                {
                    steps_remaining = 0;
                    step_change = target - value;
                }

                if (!this->interpolate)
                {
                    value = target;
                    step_change = 0;
                }
            }

            value += step_change;

            out[channel][frame] = value;

            steps_remaining--;
        }

        this->value[channel] = value;
        this->steps_remaining[channel] = steps_remaining;
        this->step_change[channel] = step_change;
    }
}

}
