#include "signalflow/core/graph.h"
#include "signalflow/node/envelope/line.h"

namespace signalflow
{

Line::Line(NodeRef start, NodeRef end, NodeRef time, NodeRef loop, NodeRef clock)
    : start(start), end(end), time(time), loop(loop), clock(clock)
{
    this->name = "line";

    this->create_input("start", this->start);
    this->create_input("end", this->end);
    this->create_input("time", this->time);
    this->create_input("loop", this->loop);
    this->create_input("clock", this->clock);

    this->alloc();

    if (!clock)
    {
        this->trigger();
    }
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
            this->step[channel] = 0;
            this->duration_samples[channel] = this->graph->get_sample_rate() * this->time->out[channel][0] - 1;
            this->value[channel] = this->start->out[channel][0];
            this->value_change_per_step[channel] = (this->end->out[channel][0] - this->start->out[channel][0]) / this->duration_samples[channel];
        }
    }
}

void Line::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(clock, channel, frame))
            {
                this->step[channel] = 0;
                this->duration_samples[channel] = this->graph->get_sample_rate() * this->time->out[channel][frame] - 1;
                this->value[channel] = this->start->out[channel][frame];
                this->value_change_per_step[channel] = (this->end->out[channel][frame] - this->start->out[channel][frame]) / this->duration_samples[channel];
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
                    this->value[channel] = this->start->out[channel][frame];
                    this->value_change_per_step[channel] = (this->end->out[channel][frame] - this->start->out[channel][frame]) / this->duration_samples[channel];
                }
            }
        }
    }
}

}
