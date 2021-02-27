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
                       NodeRef duty_cycle)
    : buffer(buffer), segment_count(segment_count), stutter_probability(stutter_probability), stutter_count(stutter_count), jump_probability(jump_probability), duty_cycle(duty_cycle)
{
    this->name = "beat-cutter";

    this->set_channels(1, buffer->get_num_channels());

    this->create_buffer("buffer", buffer);
    this->create_input("stutter_probability", this->stutter_probability);
    this->create_input("stutter_count", this->stutter_count);
    this->create_input("jump_probability", this->jump_probability);
    this->create_input("duty_cycle", this->duty_cycle);

    this->segment_offsets.resize(segment_count);
    this->segment_length = (int) ((float) this->buffer->get_num_frames() / segment_count);
    for (int i = 0; i < segment_count; i++)
    {
        this->segment_offsets[i] = roundf((float) i * this->buffer->get_num_frames() / segment_count);
    }

    this->phase = 0;
    this->segment_index = 0;
    this->segment_phase = 0;
    this->segment_duty = 1.0;
    this->current_segment_offset = this->segment_offsets[0];
    this->next_segment_offset = this->segment_offsets[1];

    this->current_stutter_length = this->segment_length;
}

void BeatCutter::process(Buffer &out, int num_frames)
{
    sample rv;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (this->segment_duty == 1 || (this->segment_phase % this->current_stutter_length) < (this->segment_duty * this->current_stutter_length))
            {
                rv = this->buffer->data[channel][this->current_segment_offset + this->segment_phase % this->current_stutter_length];
                out[channel][frame] = rv;
            }
            else
            {
                out[channel][frame] = 0.0;
            }
        }

        this->phase += 1;
        this->segment_phase += 1;
        this->phase = this->phase % this->buffer->get_num_frames();
        if (this->phase == this->next_segment_offset)
        {
            this->segment_index = (this->segment_index + 1) % this->segment_count;
            this->segment_phase = 0;
            this->current_segment_offset = this->segment_offsets[this->segment_index];
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
            this->next_segment_offset = this->segment_offsets[(this->segment_index + 1) % this->segment_count];
            this->segment_duty = this->duty_cycle->out[0][frame];
        }
    }
}

}
