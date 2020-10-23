#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/sine.h"

#include <stdio.h>

namespace signalflow
{

Sine::Sine(NodeRef frequency)
    : frequency(frequency)
{
    this->name = "sine";
    this->create_input("frequency", this->frequency);

    // This can't be done in Node::Node because base class constructors
    // cannot call subclass virtual functions (because the subclass hasn't yet)
    // been created).
    // https://isocpp.org/wiki/faq/strange-inheritance#calling-virtuals-from-ctors
    this->allocate_memory(SIGNALFLOW_MAX_CHANNELS);
}

void Sine::allocate_memory(int output_buffer_count)
{
    this->phase.resize(output_buffer_count);
}

void Sine::process(sample **out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

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
