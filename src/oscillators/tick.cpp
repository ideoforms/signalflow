#include "signal/oscillators/tick.h"
#include "signal/graph.h"

namespace libsignal
{

Tick::Tick(NodeRef frequency) :
    frequency(frequency)
{
    this->name = "tick";
    this->steps_remaining = 0;
    this->add_input("frequency", this->frequency);
}

void Tick::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample rv = 0;
        if (this->steps_remaining <= 0)
        {
            float freq_in = this->frequency->out[0][frame];
            if (freq_in > 0)
            {
                this->steps_remaining = this->graph->sample_rate / this->frequency->out[0][frame];
                rv = 1;
            }
        }
        else
        {
            this->steps_remaining--;
        }

        this->out[0][frame] = rv;
    }
}

}
