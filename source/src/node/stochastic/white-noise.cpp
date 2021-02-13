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
    : frequency(frequency), min(min), max(max), interpolate(interpolate), random_interval(random_interval), reset(reset)
{
    this->name = "white-noise";
    this->create_input("frequency", this->frequency);
    this->create_input("min", this->min);
    this->create_input("max", this->max);
    this->create_input("reset", this->reset);

    struct timeval tv;
    this->rng = gsl_rng_alloc(gsl_rng_default);

    /*--------------------------------------------------------------------*
     * Seed with current time multiplied by microsecond part, to give
     * a pretty decent non-correlated seed.
     *--------------------------------------------------------------------*/
    gettimeofday(&tv, 0);
    seed = tv.tv_sec * tv.tv_usec;
    gsl_rng_set(rng, seed);

    this->alloc();
}

void WhiteNoise::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::numeric_limits<float>::max());
    this->steps_remaining.resize(this->num_output_channels_allocated);
    this->step_change.resize(this->num_output_channels_allocated);
}

void WhiteNoise::trigger(std::string name, float value)
{
    gsl_rng_set(rng, seed);
}

void WhiteNoise::process(Buffer &out, int num_frames)
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
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(reset, channel, frame))
            {
                gsl_rng_set(rng, seed);
            }

            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];
            float frequency = this->frequency->out[channel][frame];
            if (!frequency)
                frequency = this->graph->get_sample_rate();

            if (this->steps_remaining[channel] <= 0)
            {
                // pick a new target value
                float target = min + ((max - min) * gsl_rng_uniform(this->rng));

                if (frequency > 0)
                {
                    if (random_interval)
                    {
                        this->steps_remaining[channel] = (int) (gsl_rng_uniform(this->rng) * this->graph->get_sample_rate() / (frequency / 2.0));
                    }
                    else
                    {
                        this->steps_remaining[channel] = (int) round(this->graph->get_sample_rate() / frequency);
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
