#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/triangle.h"

namespace signalflow
{

Triangle::Triangle(NodeRef frequency)
    : frequency(frequency)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "triangle";
    this->create_input("frequency", this->frequency);
    this->alloc();
}

void Triangle::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void Triangle::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float rv = (this->phase[channel] < 0.5) ? (this->phase[channel] * 4.0 - 1.0) : (1.0 - (this->phase[channel] - 0.5) * 4.0);

            out[channel][frame] = rv;

            this->phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
            while (this->phase[channel] >= 1.0)
                this->phase[channel] -= 1.0;
        }
    }
}

}
