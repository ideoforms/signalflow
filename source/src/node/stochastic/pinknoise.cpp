#include "signalflow/node/stochastic/pinknoise.h"

#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include <stdlib.h>

#include <limits>

#define SIGNALFLOW_PINKNOISE_NUM_OCTAVES 16

namespace signalflow
{

PinkNoise::PinkNoise()
{
    this->name = "pinknoise";
    this->alloc();
}

void PinkNoise::alloc()
{
    this->value.resize(this->num_output_channels_allocated, std::vector<float>(SIGNALFLOW_PINKNOISE_NUM_OCTAVES, std::numeric_limits<float>::max()));
    this->steps_remaining.resize(this->num_output_channels_allocated, std::vector<int>(SIGNALFLOW_PINKNOISE_NUM_OCTAVES));
}

void PinkNoise::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = 0;
            for (int octave = 0; octave < SIGNALFLOW_PINKNOISE_NUM_OCTAVES; octave++)
            {
                if (this->steps_remaining[channel][octave] <= 0)
                {
                    // pick a new target value
                    float target = random_uniform(-1, 1);

                    this->steps_remaining[channel][octave] = powf(2, octave);
                    if (this->steps_remaining[channel][octave] == 0)
                        this->steps_remaining[channel][octave] = 1;

                    this->value[channel][octave] = target;
                }

                out[channel][frame] += this->value[channel][octave] / SIGNALFLOW_PINKNOISE_NUM_OCTAVES;

                this->steps_remaining[channel][octave]--;
            }
        }
    }
}

}
