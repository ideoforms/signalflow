#include "signal/oscillators/sine.h"
#include "signal/graph.h"

#include <math.h>
#include <stdio.h>

namespace libsignal
{

Sine::Sine(NodeRef frequency) : frequency(frequency)
{
    this->name = "sine";
    this->add_input("frequency", this->frequency);
    memset(this->phase, 0, sizeof(this->phase));
}

void Sine::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float freq = this->frequency->out[channel][frame];
            out[channel][frame] = sin(this->phase[channel] * M_PI * 2.0);
            this->phase[channel] += freq / this->graph->sample_rate;

            while (this->phase[channel] > 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
