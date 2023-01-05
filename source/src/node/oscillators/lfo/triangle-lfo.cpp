#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/triangle-lfo.h"

namespace signalflow
{

TriangleLFO::TriangleLFO(NodeRef frequency, NodeRef min, NodeRef max, NodeRef phase)
    : LFO(frequency, min, max, phase)
{
    this->name = "triangle-lfo";
}

void TriangleLFO::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];
            float phase = fmod(this->current_phase[channel] + this->phase->out[channel][frame], 1);
            float rv = (phase < 0.5) ? (phase * 2.0) : (1.0 - (phase - 0.5) * 2.0);
            out[channel][frame] = min + rv * (max - min);

            this->current_phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
        }
        while (this->current_phase[channel] >= 1.0)
            this->current_phase[channel] -= 1.0;
    }
}

}
