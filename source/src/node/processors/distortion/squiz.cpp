#include "signalflow/core/graph.h"
#include "signalflow/node/processors/distortion/squiz.h"

namespace signalflow
{

// Max length of a single chunk, matching SuperCollider's default memlen of 0.1s
float SIGNALFLOW_SQUIZ_MAX_CHUNK_DURATION = 0.1;

Squiz::Squiz(NodeRef input, NodeRef rate, NodeRef chunk_size)
    : UnaryOpNode(input), rate(rate), chunk_size(chunk_size)
{
    this->name = "squiz";
    this->create_input("rate", this->rate);
    this->create_input("chunk_size", this->chunk_size);

    SIGNALFLOW_CHECK_GRAPH();
    this->buffer_length = (int) (SIGNALFLOW_SQUIZ_MAX_CHUNK_DURATION * this->graph->get_sample_rate());
    this->buffers.resize(SIGNALFLOW_MAX_CHANNELS, std::vector<sample>(this->buffer_length, 0.0));
    this->write_pos.resize(SIGNALFLOW_MAX_CHANNELS, 0);
    this->read_pos.resize(SIGNALFLOW_MAX_CHANNELS, 0.0);
    this->last_input.resize(SIGNALFLOW_MAX_CHANNELS, 0.0);
    this->zero_crossings.resize(SIGNALFLOW_MAX_CHANNELS, 0);
}

void Squiz::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        std::vector<sample> &buf = this->buffers[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            float rate = std::min(std::max(this->rate->out[channel][frame], 1.0f), (float) this->buffer_length);
            int chunk_size = (int) this->chunk_size->out[channel][frame];
            sample input = this->input->out[channel][frame];

            // Play back the buffer at `rate`, then silence once the end is reached
            int read_index = (int) this->read_pos[channel];
            if (read_index < this->buffer_length)
            {
                out[channel][frame] = buf[read_index];
                this->read_pos[channel] += rate;
            }
            else
            {
                out[channel][frame] = 0.0;
            }

            // Start a new chunk on every Nth positive-going zero crossing, or when the buffer is full
            this->write_pos[channel]++;
            if (this->write_pos[channel] == this->buffer_length
                || (this->last_input[channel] < 0 && input >= 0 && ++this->zero_crossings[channel] >= chunk_size))
            {
                this->write_pos[channel] = 0;
                this->read_pos[channel] = 0.0;
                this->zero_crossings[channel] = 0;
            }
            buf[this->write_pos[channel]] = input;
            this->last_input[channel] = input;
        }
    }
}

}
