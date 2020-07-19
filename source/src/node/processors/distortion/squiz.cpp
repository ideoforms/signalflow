#include "signalflow/node/processors/distortion/squiz.h"

namespace signalflow
{

int SIGNAL_SQUIZ_LOOKAHEAD_FRAMES = 2205;

Squiz::Squiz(NodeRef input, NodeRef rate, NodeRef chunk_size)
    : UnaryOpNode(input), rate(rate), chunk_size(chunk_size)
{
    this->name = "squiz";
    this->create_input("rate", this->rate);
    this->create_input("chunk_size", this->chunk_size);

    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        BufferRef buf = new Buffer(1, SIGNAL_SQUIZ_LOOKAHEAD_FRAMES);
        this->buffers.push_back(buf);
        this->write_pos.push_back(SIGNAL_SQUIZ_LOOKAHEAD_FRAMES / 2);
        this->read_pos.push_back(0.0f);
        this->read_pos_chunk_counter.push_back(0);
        this->repitched_read_pos.push_back(0.0f);
        this->repitched_read_pos_chunk_counter.push_back(0);
    }
}

void Squiz::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (this->repitched_read_pos_chunk_counter[channel] < this->chunk_size->out[channel][frame])
            {
                sample read_last = this->buffers[channel]->get(this->repitched_read_pos[channel]);
                this->repitched_read_pos[channel] += this->rate->out[channel][frame];
                if (this->repitched_read_pos[channel] >= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES)
                    this->repitched_read_pos[channel] -= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES;
                sample read_cur = this->buffers[channel]->get(this->repitched_read_pos[channel]);
                if (read_cur > 0 && read_last <= 0)
                {
                    this->repitched_read_pos_chunk_counter[channel]++;
                }

                if (this->repitched_read_pos_chunk_counter[channel] < this->chunk_size->out[channel][frame])
                {
                    this->out[channel][frame] = read_cur;
                }
                else
                {
                    this->out[channel][frame] = 0.0;
                }
            }
            else
            {
                this->out[channel][frame] = 0.0;
            }

            sample read_last = this->buffers[channel]->get(this->read_pos[channel]);
            this->read_pos[channel] += 1;
            if (this->read_pos[channel] >= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES)
                this->read_pos[channel] -= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES;
            sample read_cur = this->buffers[channel]->get(this->read_pos[channel]);

            if (read_cur > 0 && read_last <= 0)
            {
                this->read_pos_chunk_counter[channel] += 1;

                // if (this->read_pos_chunk_counter[channel] >= this->chunk_size->out[channel][frame])
                if (this->read_pos_chunk_counter[channel] >= 1)
                {
                    this->read_pos_chunk_counter[channel] = 0;
                    this->repitched_read_pos_chunk_counter[channel] = 0;
                    this->repitched_read_pos[channel] = this->read_pos[channel];
                }
            }

            this->buffers[channel]->data[0][this->write_pos[channel]] = this->input->out[channel][frame];

            this->write_pos[channel] += 1;
            if (this->write_pos[channel] >= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES)
                this->write_pos[channel] -= SIGNAL_SQUIZ_LOOKAHEAD_FRAMES;
        }
    }
}

}
