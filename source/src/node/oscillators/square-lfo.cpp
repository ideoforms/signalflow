#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/square-lfo.h"

namespace signalflow
{

SquareLFO::SquareLFO(NodeRef frequency, NodeRef min, NodeRef max, NodeRef width)
    : LFO(frequency, min, max), width(width)
{
    this->name = "square-lfo";
    this->create_input("width", this->width);
}

void SquareLFO::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[channel][frame];
            float width = this->width->out[channel][frame];
            float rv = (this->phase[channel] < width) ? this->max->out[channel][frame] : this->min->out[channel][frame];

            out[channel][frame] = rv;

            this->phase[channel] += 1.0 / (this->graph->get_sample_rate() / frequency);
            if (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
