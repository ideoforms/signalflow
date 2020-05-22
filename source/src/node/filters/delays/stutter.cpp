#include "signal/node/filters/delays/stutter.h"
#include "signal/core/graph.h"
#include "signal/node/oscillators/constant.h"

namespace libsignal
{

Stutter::Stutter(NodeRef input, NodeRef stutter_time, NodeRef stutter_count, NodeRef clock, float max_stutter_time)
    : UnaryOpNode(input), stutter_time(stutter_time), stutter_count(stutter_count), clock(clock), max_stutter_time(max_stutter_time)
{
    this->name = "stutter";
    this->add_input("stutter_time", this->stutter_time);
    this->add_input("stutter_count", this->stutter_count);
    this->add_input("clock", this->clock);

    this->stutter_index.resize(SIGNAL_MAX_CHANNELS);
    this->stutter_sample_buffer_offset.resize(SIGNAL_MAX_CHANNELS);
    this->stutters_to_do.resize(SIGNAL_MAX_CHANNELS);
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        this->stutters_to_do[i] = 0;
    }
    this->stutter_samples_remaining.resize(SIGNAL_MAX_CHANNELS);

    SIGNAL_CHECK_GRAPH();
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        buffers.push_back(new SampleRingBuffer(max_stutter_time * this->graph->get_sample_rate()));
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
    if (name != SIGNAL_DEFAULT_TRIGGER)
    {
        throw std::runtime_error("Unknown trigger: " + name);
    }
    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        this->stutter_index[channel] = 0;
        this->stutters_to_do[channel] = this->stutter_count->out[channel][0];
        this->stutter_samples_remaining[channel] = this->stutter_time->out[channel][0] * graph->get_sample_rate();
        this->stutter_sample_buffer_offset[channel] = stutter_samples_remaining[channel];
    }
}

void Stutter::process(sample **out, int num_frames)
{
    SIGNAL_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            SIGNAL_PROCESS_TRIGGER(this->clock, frame, SIGNAL_DEFAULT_TRIGGER);

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
                    this->out[channel][frame] = this->input->out[channel][frame];
                }
                else
                {
                    // TODO this won't quite work
                    int buffer_sample_offset = -this->stutter_samples_remaining[channel];
                    this->out[channel][frame] = this->buffers[channel]->get(buffer_sample_offset);
                }

            }
            else
            {
                // not stuttering, just output our input
                this->out[channel][frame] = this->input->out[channel][frame];
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
