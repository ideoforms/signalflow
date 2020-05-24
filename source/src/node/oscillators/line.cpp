#include "signal/node/oscillators/line.h"
#include "signal/core/graph.h"

namespace libsignal
{

Line::Line(NodeRef from, NodeRef to, NodeRef time)
    : from(from), to(to), time(time)
{
    this->name = "line";

    this->add_input("from", this->from);
    this->add_input("to", this->to);
    this->add_input("time", this->time);

    this->value = 0.0;
    this->value_change_per_step = 0.0;
    this->step = 0;
    this->duration_samples = 0;
}

void Line::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (!duration_samples)
            {
                float from = this->from->out[channel][frame];
                float to = this->to->out[channel][frame];
                float time = this->time->out[channel][frame];

                this->duration_samples = this->graph->get_sample_rate() * time - 1;
                this->value = from;
                this->value_change_per_step = (to - from) / this->duration_samples;
            }

            out[channel][frame] = this->value;

            if (this->step < this->duration_samples)
            {
                this->value += this->value_change_per_step;
                this->step++;
            }
        }
    }
}

}
