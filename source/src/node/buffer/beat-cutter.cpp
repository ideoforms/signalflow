#include "signalflow/core/random.h"
#include "signalflow/node/buffer/beat-cutter.h"

#include <stdlib.h>

namespace signalflow
{

BeatCutter::BeatCutter(BufferRef buffer,
                       int segment_count,
                       NodeRef stutter_probability,
                       NodeRef stutter_count,
                       NodeRef jump_probability,
                       NodeRef duty_cycle,
                       NodeRef rate,
                       NodeRef segment_rate)
    : buffer(buffer), segment_count(segment_count), stutter_probability(stutter_probability), stutter_count(stutter_count), jump_probability(jump_probability), duty_cycle(duty_cycle), rate(rate), segment_rate(segment_rate)
{
    this->name = "beat-cutter";

    this->create_input("stutter_probability", this->stutter_probability);
    this->create_input("stutter_count", this->stutter_count);
    this->create_input("jump_probability", this->jump_probability);
    this->create_input("duty_cycle", this->duty_cycle);
    this->create_input("rate", this->rate);
    this->create_input("segment_rate", this->segment_rate);

    this->segment_offsets.resize(segment_count);

    this->phase = 0;
    this->segment_index = 0;
    this->segment_phase = 0;
    this->segment_duty = 1.0;
    this->current_segment_offset = 0;
    this->next_segment_offset = 0;
    this->current_stutter_length = 0;
    this->current_segment_rate = 1.0;

    this->create_buffer("buffer", this->buffer);
    this->set_channels(1, 0);
    if (buffer)
    {
        this->set_buffer("buffer", buffer);
    }
}

void BeatCutter::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->Node::set_buffer(name, buffer);
        this->num_output_channels = buffer->get_num_channels();

        this->segment_length = (int) ((float) this->buffer->get_num_frames() / this->segment_count);
        for (int i = 0; i < this->segment_count; i++)
        {
            this->segment_offsets[i] = roundf((float) i * this->buffer->get_num_frames() / this->segment_count);
        }

        this->current_segment_offset = this->segment_offsets[0];
        this->next_segment_offset = this->segment_offsets[1];
        this->current_stutter_length = this->segment_length;
    }
}

void BeatCutter::set_segment(int segment_index, int frame)
{
    this->segment_index = segment_index;
    this->current_segment_offset = this->segment_offsets[segment_index];
    int next_segment_index = (segment_index + 1) % this->segment_count;
    this->next_segment_offset = this->segment_offsets[next_segment_index];
    // This is needed to track when phase has exceeded the next segment offset
    if (this->next_segment_offset == 0)
        this->next_segment_offset = this->buffer->get_num_frames();

    this->segment_phase = 0.0;

    /*--------------------------------------------------------------------------------
     * Take snapshots of parameters that are modulated and captured at the start
     * of each segment:
     *  - segment_rate
     *  - duty_cycle
     *  - jump
     *  - stutter
     *--------------------------------------------------------------------------------*/
    this->current_segment_rate = this->segment_rate->out[0][frame];
    this->segment_duty = this->duty_cycle->out[0][frame];
    if (random_uniform() < this->jump_probability->out[0][frame])
    {
        // jump
        this->current_segment_offset = this->segment_offsets[random_integer(0, this->segment_count)];
    }
    if (random_uniform() < this->stutter_probability->out[0][frame])
    {
        // stutter
        float current_stutter_count = this->stutter_count->out[0][frame];
        this->current_stutter_length = (int) (this->segment_length / current_stutter_count);
    }
    else
    {
        this->current_stutter_length = this->segment_length;
    }
}

void BeatCutter::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_TRIGGER_PLAY_SEGMENT)
    {
        int segment_index = (int) value;
        if (segment_index < this->segment_count)
        {
            this->set_segment(segment_index);
            this->phase = this->current_segment_offset;
        }
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

void BeatCutter::process(Buffer &out, int num_frames)
{
    if (!this->buffer)
    {
        return;
    }

    sample rv;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (this->segment_duty == 1 || (fmod(this->segment_phase, this->current_stutter_length)) < (this->segment_duty * this->current_stutter_length))
            {
                rv = this->buffer->get(channel, this->current_segment_offset + fmod(this->segment_phase, this->current_stutter_length));
                out[channel][frame] = rv;
            }
            else
            {
                out[channel][frame] = 0.0;
            }
        }

        this->phase += this->rate->out[0][frame];
        this->segment_phase += this->rate->out[0][frame] * current_segment_rate;

        if (this->phase >= this->next_segment_offset)
        {
            this->set_segment((this->segment_index + 1) % this->segment_count, frame);
        }
        this->phase = fmod(this->phase, this->buffer->get_num_frames());
    }
}

}
