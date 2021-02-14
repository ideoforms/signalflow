#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/line.h"

namespace signalflow
{

Line::Line(NodeRef from, NodeRef to, NodeRef time, NodeRef loop)
    : from(from), to(to), time(time), loop(loop)
{
    this->name = "line";

    this->create_input("from", this->from);
    this->create_input("to", this->to);
    this->create_input("time", this->time);
    this->create_input("loop", this->loop);

    this->alloc();
}

void Line::alloc()
{
    this->value.resize(this->num_output_channels_allocated);
    this->value_change_per_step.resize(this->num_output_channels_allocated);
    this->step.resize(this->num_output_channels_allocated);
    this->duration_samples.resize(this->num_output_channels_allocated);
}

void Line::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->duration_samples[channel] = 0.0;
            this->step[channel] = 0;
        }
    }
}

void Line::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (!duration_samples[channel])
            {
                this->duration_samples[channel] = this->graph->get_sample_rate() * this->time->out[channel][frame] - 1;
                this->value[channel] = this->from->out[channel][frame];
                this->value_change_per_step[channel] = (this->to->out[channel][frame] - this->from->out[channel][frame]) / this->duration_samples[channel];
            }

            out[channel][frame] = this->value[channel];

            if (this->step[channel] < this->duration_samples[channel])
            {
                this->value[channel] += this->value_change_per_step[channel];
                this->step[channel]++;
            }
            else
            {
                if (this->loop->out[channel][frame])
                {
                    this->step[channel] = 0;
                    this->duration_samples[channel] = this->graph->get_sample_rate() * this->time->out[channel][frame] - 1;
                    this->value[channel] = this->from->out[channel][frame];
                    this->value_change_per_step[channel] = (this->to->out[channel][frame] - this->from->out[channel][frame]) / this->duration_samples[channel];
                }
            }
        }
    }
}

}
