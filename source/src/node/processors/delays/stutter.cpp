#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/stutter.h"

namespace signalflow
{

Stutter::Stutter(NodeRef input, NodeRef stutter_time, NodeRef stutter_count, NodeRef clock, float max_stutter_time)
    : UnaryOpNode(input), stutter_time(stutter_time), stutter_count(stutter_count), clock(clock), max_stutter_time(max_stutter_time)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "stutter";
    this->create_input("stutter_time", this->stutter_time);
    this->create_input("stutter_count", this->stutter_count);
    this->create_input("clock", this->clock);
    this->alloc();
}

void Stutter::alloc()
{
    this->stutter_index.resize(this->num_output_channels_allocated);
    this->stutter_sample_buffer_offset.resize(this->num_output_channels_allocated);
    this->stutters_to_do.resize(this->num_output_channels_allocated);
    this->stutter_samples_remaining.resize(this->num_output_channels_allocated);

    int buffers_to_allocate = this->num_output_channels_allocated - buffers.size();
    for (int i = 0; i < buffers_to_allocate; i++)
    {
        buffers.push_back(new SampleRingBuffer(this->max_stutter_time * this->graph->get_sample_rate()));
    }
}

Stutter::~Stutter()
{
    for (auto buffer : buffers)
    {
        delete buffer;
    }
}

void Stutter::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_input_channels; channel++)
        {
            this->stutter_index[channel] = 0;
            this->stutters_to_do[channel] = this->stutter_count->out[channel][0];
            this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][0] * graph->get_sample_rate();
            this->stutter_sample_buffer_offset[channel] = stutter_samples_remaining[channel];
        }
    }
}

void Stutter::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (SIGNALFLOW_CHECK_CHANNEL_TRIGGER(this->clock, channel, frame))
            {
                this->stutter_index[channel] = 0;
                this->stutters_to_do[channel] = this->stutter_count->out[channel][0];
                this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][0] * graph->get_sample_rate();
                this->stutter_sample_buffer_offset[channel] = stutter_samples_remaining[channel];
            }

            if (this->stutters_to_do[channel] > 0)
            {
                this->stutter_samples_remaining[channel]--;

                if (this->stutter_samples_remaining[channel] <= 0)
                {
                    this->stutter_index[channel] += 1;
                    if (this->stutter_index[channel] > this->stutters_to_do[channel])
                    {
                        // stuttering over
                        this->stutters_to_do[channel] = 0;
                        this->stutter_index[channel] = 0;
                    }
                    else
                    {
                        this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][frame] * graph->get_sample_rate();
                    }
                }

                if (this->stutter_index[channel] == 0)
                {
                    out[channel][frame] = this->input->out[channel][frame];
                }
                else
                {
                    // TODO this won't quite work
                    int buffer_sample_offset = -this->stutter_samples_remaining[channel];
                    out[channel][frame] = this->buffers[channel]->get(buffer_sample_offset);
                }
            }
            else
            {
                // not stuttering, just output our input
                out[channel][frame] = this->input->out[channel][frame];
            }

            if (this->stutter_index[channel] == 0)
            {
                // stutter_index is zero in the first stutter or when we're not stuttering
                this->buffers[channel]->append(this->input->out[channel][frame]);
            }
        }
    }
}

}
