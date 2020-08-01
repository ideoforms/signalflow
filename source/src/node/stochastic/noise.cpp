#include "signalflow/node/stochastic/noise.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include <stdlib.h>

#include <limits>

namespace signalflow
{

Noise::Noise(NodeRef frequency, bool interpolate, NodeRef min, NodeRef max)
    : frequency(frequency), min(min), max(max)
{
    this->name = "noise";
    this->create_input("frequency", this->frequency);
    this->create_input("min", this->min);
    this->create_input("max", this->max);

    this->interpolate = interpolate;

    for (int i = 0; i < SIGNALFLOW_MAX_CHANNELS; i++)
        this->value[i] = std::numeric_limits<float>::max();
    memset(this->steps_remaining, 0, sizeof(int) * SIGNALFLOW_MAX_CHANNELS);
    memset(this->step_change, 0, sizeof(int) * SIGNALFLOW_MAX_CHANNELS);
}

void Noise::process(sample **out, int num_frames)
{
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
            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];
            float frequency = this->frequency->out[channel][frame];
            if (!frequency)
                frequency = this->graph->get_sample_rate();

            if (this->steps_remaining[channel] <= 0)
            {
                // pick a new target value
                float target = random_uniform(min, max);

                if (frequency > 0)
                {
                    this->steps_remaining[channel] = random_integer(0, this->graph->get_sample_rate() / (frequency / 2.0));
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
