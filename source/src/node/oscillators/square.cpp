#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/square.h"

namespace signalflow
{

Square::Square(NodeRef frequency, NodeRef width)
    : frequency(frequency), width(width)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "square";

    this->create_input("frequency", this->frequency);
    this->create_input("width", this->width);

    this->alloc();
}

void Square::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void Square::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = this->frequency->out[channel][frame];
            float width = this->width->out[channel][frame];
            float rv = (this->phase[channel] < width) ? 1 : -1;

            out[channel][frame] = rv;

            this->phase[channel] += 1.0 / (this->graph->get_sample_rate() / frequency);
            if (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
