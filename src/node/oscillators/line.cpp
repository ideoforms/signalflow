#include "signal/node/oscillators/line.h"
#include "signal/core/graph.h"

namespace libsignal
{

Line::Line(NodeRef time, NodeRef from, NodeRef to)
    : time(time), from(from), to(to)
{
    this->name = "line";

    this->add_input("time", this->time);
    this->add_input("from", this->from);
    this->add_input("to", this->to);

    this->value = 0.0;
    this->value_change_per_step = 0.0;
    this->step = 0;
    this->step_target = 0;
}

void Line::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (!step_target)
            {
                float from = this->from->out[channel][frame];
                float to = this->to->out[channel][frame];
                float time = this->time->out[channel][frame];

                this->step_target = this->graph->sample_rate * time;
                this->value = from;
                this->value_change_per_step = (to - from) / this->step_target;
            }

            if (this->step < this->step_target)
            {
                this->value += this->value_change_per_step;
                this->step++;
            }

            this->out[channel][frame] = this->value;
        }
    }
}

}
