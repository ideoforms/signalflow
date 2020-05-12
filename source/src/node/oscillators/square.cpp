#include "signal/node/oscillators/square.h"
#include "signal/core/graph.h"

namespace libsignal
{

Square::Square(NodeRef frequency, NodeRef width)
    : frequency(frequency), width(width)
{
    this->name = "square";
    this->add_input("frequency", this->frequency);
    this->add_input("width", this->width);
    memset(this->phase, 0, sizeof(this->phase));
}

void Square::process(sample **out, int num_frames)
{
    SIGNAL_CHECK_GRAPH()

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
