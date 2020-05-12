#include "signal/node/oscillators/sine.h"
#include "signal/core/graph.h"

#include <stdio.h>

namespace libsignal
{

Sine::Sine(NodeRef frequency)
    : frequency(frequency)
{
    this->name = "sine";
    this->add_input("frequency", this->frequency);
    this->allocate_memory();
}

void Sine::allocate_memory()
{
    this->phase = new float[this->num_output_channels]();
}

void Sine::free_memory()
{
    delete [] this->phase;
}

void Sine::process(sample **out, int num_frames)
{
    SIGNAL_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float freq = this->frequency->out[channel][frame];
            out[channel][frame] = sin(this->phase[channel] * M_PI * 2.0);
            this->phase[channel] += freq / this->graph->get_sample_rate();

            while (this->phase[channel] > 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
