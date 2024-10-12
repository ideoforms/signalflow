#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/delays/stutter.h"

namespace signalflow
{

Stutter::Stutter(NodeRef input,
                 NodeRef stutter_time,
                 NodeRef stutter_count,
                 NodeRef stutter_probability,
                 NodeRef stutter_advance_time,
                 NodeRef clock,
                 float max_stutter_time)
    : UnaryOpNode(input), stutter_time(stutter_time), stutter_count(stutter_count), stutter_probability(stutter_probability), stutter_advance_time(stutter_advance_time), clock(clock), max_stutter_time(max_stutter_time)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "stutter";
    this->create_input("stutter_time", this->stutter_time);
    this->create_input("stutter_count", this->stutter_count);
    this->create_input("stutter_probability", this->stutter_probability);
    this->create_input("stutter_advance_time", this->stutter_advance_time);
    this->create_input("clock", this->clock);
    this->alloc();
}

void Stutter::alloc()
{
    this->stutter_index.resize(this->num_output_channels_allocated);
    this->stutter_sample_buffer_offset.resize(this->num_output_channels_allocated);
    this->stutters_to_do.resize(this->num_output_channels_allocated);
    this->stutter_samples_remaining.resize(this->num_output_channels_allocated);
    this->stutter_sample_index.resize(this->num_output_channels_allocated);

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
        float probability = this->stutter_probability->out[0][0];
        if (random_coin(probability))
        {
            for (int channel = 0; channel < this->num_input_channels; channel++)
            {
                this->stutter_index[channel] = 0;
                this->stutters_to_do[channel] = this->stutter_count->out[channel][0];
                this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][0] * graph->get_sample_rate();
                this->stutter_sample_buffer_offset[channel] = stutter_samples_remaining[channel];
                this->stutter_sample_index[channel] = 0;
            }
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
                float probability = this->stutter_probability->out[channel][frame];
                if (random_coin(probability))
                {
                    this->stutter_index[channel] = 0;
                    this->stutters_to_do[channel] = this->stutter_count->out[channel][0];
                    this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][frame] * graph->get_sample_rate();
                    this->stutter_sample_buffer_offset[channel] = stutter_samples_remaining[channel];
                    this->stutter_sample_index[channel] = 0;
                }
            }

            if (this->stutters_to_do[channel] > 0)
            {
                this->stutter_samples_remaining[channel]--;

                this->stutter_sample_index[channel] += 1;

                if (this->stutter_samples_remaining[channel] <= 0)
                {
                    // one of the stutters has finished
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
                    float stutter_advance_samples = (this->stutter_advance_time->out[channel][frame] * this->stutter_index[channel]) * this->graph->get_sample_rate();
                    int buffer_sample_offset = -this->stutter_samples_remaining[channel] + stutter_advance_samples - this->stutter_sample_index[channel];
                    out[channel][frame] = this->buffers[channel]->get(buffer_sample_offset);
                }
            }
            else
            {
                // not stuttering, just output our input
                out[channel][frame] = this->input->out[channel][frame];
            }

            this->buffers[channel]->append(this->input->out[channel][frame]);

        } // foreach frame
    }     // foreach channel
}

}
