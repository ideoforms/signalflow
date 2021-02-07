#include "signalflow/node/stochastic/pink-noise.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

#include <math.h>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 * This code implements the stochastic Voss-McCartney algorithm, which sums
 * multiple octaves of white noise.
 *
 * Details:
 * https://www.firstpr.com.au/dsp/pink-noise/
 * https://www.dsprelated.com/showarticle/908.php
 *--------------------------------------------------------------------------------*/

PinkNoise::PinkNoise(float low_cutoff, float high_cutoff)
{
    this->name = "pinknoise";

    SIGNALFLOW_CHECK_GRAPH();
    this->num_octaves = (int) ceilf(log2(high_cutoff / low_cutoff));
    this->initial_octave = (int) floor(log2((graph->get_sample_rate() / 2) / high_cutoff));

    this->alloc();
}

void PinkNoise::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::vector<float>(this->num_octaves, std::numeric_limits<float>::max()));
    this->steps_remaining.resize(this->num_output_channels_allocated, std::vector<int>(this->num_octaves));
}

void PinkNoise::process(Buffer &out, int num_frames)
{
    float scale_factor = 1.0 / (2.0f * sqrtf(this->num_octaves));

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = 0;
            for (int octave = this->initial_octave; octave < this->initial_octave + this->num_octaves; octave++)
            {
                if (this->steps_remaining[channel][octave] <= 0)
                {
                    // pick a new target value
                    float target = random_uniform(-1, 1);

                    this->steps_remaining[channel][octave] = random_integer(0, powf(2, octave + this->initial_octave) * 2);
                    if (this->steps_remaining[channel][octave] == 0)
                        this->steps_remaining[channel][octave] = 1;

                    this->value[channel][octave] = target;
                }

                out[channel][frame] += this->value[channel][octave] / this->num_octaves;

                this->steps_remaining[channel][octave]--;
            }
            out[channel][frame] *= scale_factor;
        }
    }
}

}
