#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/sine-lfo.h"

namespace signalflow
{

SineLFO::SineLFO(NodeRef frequency, NodeRef min, NodeRef max)
    : LFO(frequency, min, max)
{
    this->name = "sine-lfo";
}

void SineLFO::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float freq = this->frequency->out[channel][frame];
            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];
            out[channel][frame] = min + ((sin(this->phase[channel] * M_PI * 2.0) + 1) / 2) * (max - min);
            this->phase[channel] += freq / this->graph->get_sample_rate();

            while (this->phase[channel] > 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
