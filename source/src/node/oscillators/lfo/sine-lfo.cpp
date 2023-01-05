#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/sine-lfo.h"

namespace signalflow
{

SineLFO::SineLFO(NodeRef frequency, NodeRef min, NodeRef max, NodeRef phase)
    : LFO(frequency, min, max, phase)
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
            float phase = fmod(this->current_phase[channel] + this->phase->out[channel][frame], 1);
            out[channel][frame] = min + ((sin(phase * M_PI * 2.0) + 1) / 2) * (max - min);
            this->current_phase[channel] += freq / this->graph->get_sample_rate();
        }
        while (this->current_phase[channel] >= 1.0)
            this->current_phase[channel] -= 1.0;
    }
}

}
