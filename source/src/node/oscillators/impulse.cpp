#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/impulse.h"
#include <limits.h>

namespace signalflow
{

Impulse::Impulse(NodeRef frequency)
    : frequency(frequency)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "impulse";
    this->create_input("frequency", this->frequency);

    this->alloc();
}

void Impulse::alloc()
{
    this->steps_remaining.resize(this->num_output_channels_allocated);
}

void Impulse::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            sample rv = 0;
            if (this->steps_remaining[channel] <= 0)
            {
                rv = 1;
                float freq_in = this->frequency->out[channel][frame];
                if (freq_in > 0)
                {
                    this->steps_remaining[channel] = this->graph->get_sample_rate() / this->frequency->out[channel][frame];
                }
                else
                {
                    this->steps_remaining[channel] = INT_MAX;
                }
            }

            this->steps_remaining[channel]--;

            out[channel][frame] = rv;
        }
    }
}

}
