#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/line.h"

namespace signalflow
{

Line::Line(NodeRef from, NodeRef to, NodeRef time)
    : from(from), to(to), time(time)
{
    this->name = "line";

    this->create_input("from", this->from);
    this->create_input("to", this->to);
    this->create_input("time", this->time);

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
                float from = this->from->out[channel][frame];
                float to = this->to->out[channel][frame];
                float time = this->time->out[channel][frame];

                this->duration_samples[channel] = this->graph->get_sample_rate() * time - 1;
                this->value[channel] = from;
                this->value_change_per_step[channel] = (to - from) / this->duration_samples[channel];
            }

            out[channel][frame] = this->value[channel];

            if (this->step[channel] < this->duration_samples[channel])
            {
                this->value[channel] += this->value_change_per_step[channel];
                this->step[channel]++;
            }
        }
    }
}

}
