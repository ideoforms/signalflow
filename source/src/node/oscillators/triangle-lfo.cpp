#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/triangle-lfo.h"

namespace signalflow
{

TriangleLFO::TriangleLFO(NodeRef frequency, NodeRef min, NodeRef max)
    : LFO(frequency, min, max)
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

            float rv = (this->phase[channel] < 0.5) ? (this->phase[channel] * 2.0) : (1.0 - (this->phase[channel] - 0.5) * 2.0);
            out[channel][frame] = min + rv * (max - min);

            this->phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
