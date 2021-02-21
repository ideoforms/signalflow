#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/saw-lfo.h"

namespace signalflow
{

SawLFO::SawLFO(NodeRef frequency, NodeRef min, NodeRef max)
    : LFO(frequency, min, max)
{
    this->name = "saw-lfo";
}

void SawLFO::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];

            out[channel][frame] = min + this->phase[channel] * (max - min);

            this->phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
