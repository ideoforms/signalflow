#include "signal/node/filters/squiz.h"

namespace libsignal
{

int LOOKAHEAD_FRAMES = 44100;

Squiz::Squiz(NodeRef input, NodeRef rate, NodeRef chunk_size)
    : UnaryOpNode(input), rate(rate), chunk_size(chunk_size)
{
    this->name = "squiz";
    this->add_input("rate", this->rate);
    this->add_input("chunk_size", this->chunk_size);

    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        BufferRef buf = new Buffer(1, LOOKAHEAD_FRAMES);
        this->buffers.push_back(buf);
        this->write_pos.push_back(LOOKAHEAD_FRAMES / 2);
        this->read_pos.push_back(0.0f);
        this->read_pos_accelerated.push_back(0.0f);
        this->chunk_counter.push_back(0);
    }
}

void Squiz::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (this->chunk_counter[channel] < this->chunk_size->out[channel][frame])
            {
                sample read_last = this->buffers[channel]->get(this->read_pos_accelerated[channel]);
                this->read_pos_accelerated[channel] += this->rate->out[channel][frame];
                if (this->read_pos_accelerated[channel] >= LOOKAHEAD_FRAMES)
                    this->read_pos_accelerated[channel] -= LOOKAHEAD_FRAMES;
                sample read_cur = this->buffers[channel]->get(this->read_pos_accelerated[channel]);
                if (read_cur > 0 && read_last <= 0)
                {
                    this->chunk_counter[channel]++;
                    this->out[channel][frame] = 0.0;
                }
                else
                {
                    this->out[channel][frame] = read_cur;
                }
            }
            else
            {
                this->out[channel][frame] = 0.0;
            }

            sample read_last = this->buffers[channel]->get(this->read_pos[channel]);
            this->read_pos[channel] += 1;
            if (this->read_pos[channel] >= LOOKAHEAD_FRAMES)
                this->read_pos[channel] -= LOOKAHEAD_FRAMES;
            sample read_cur = this->buffers[channel]->get(this->read_pos[channel]);

            if (read_cur > 0 && read_last <= 0)
            {
                this->chunk_counter[channel] = false;
                this->read_pos_accelerated[channel] = this->read_pos[channel];
            }

            this->buffers[channel]->data[0][this->write_pos[channel]] = this->input->out[channel][frame];

            this->write_pos[channel] += 1;
            if (this->write_pos[channel] >= LOOKAHEAD_FRAMES)
                this->write_pos[channel] -= LOOKAHEAD_FRAMES;
        }
    }
}

}
