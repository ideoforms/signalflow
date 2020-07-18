#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/buffer-player.h"

#include <stdlib.h>

namespace signalflow
{

BufferPlayer::BufferPlayer(BufferRef buffer, NodeRef rate, NodeRef loop)
    : rate(rate), loop(loop)
{
    SIGNAL_CHECK_GRAPH();

    this->name = "buffer-player";

    this->add_buffer("buffer", this->buffer);
    this->add_input("rate", this->rate);
    this->add_input("loop_start", this->loop_start);
    this->add_input("loop_end", this->loop_end);
    this->add_input("loop", this->loop);

    this->phase = 0.0;

    this->buffer = buffer;
    this->num_input_channels = 1;
    this->num_output_channels = 0;

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a BufferPlayer is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }

    this->min_input_channels = this->max_input_channels = 0;
    this->min_output_channels = this->max_output_channels = this->num_output_channels;

    this->trigger();
}

void BufferPlayer::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        Node::set_buffer(name, buffer);
        this->num_output_channels = buffer->get_num_channels();
    }
}

void BufferPlayer::process(sample **out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    sample s;

    int loop_start = this->loop_start ? (buffer->get_num_frames() * this->loop_start->out[0][0]) : 0;
    int loop_end = this->loop_end ? (buffer->get_num_frames() * this->loop_end->out[0][0]) : buffer->get_num_frames();

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if ((int) this->phase < loop_end)
            {
                s = this->buffer->data[channel][(int) this->phase];
            }
            else
            {
                if (loop->out[channel][frame])
                {
                    this->phase = loop_start;
                    s = this->buffer->data[channel][(int) this->phase];
                }
                else
                {
                    if (this->state == SIGNAL_NODE_STATE_ACTIVE)
                    {
                        this->set_state(SIGNAL_NODE_STATE_FINISHED);
                    }
                    s = 0.0;
                }
            }

            out[channel][frame] = s;
        }

        this->phase += this->rate->out[0][frame];
    }
}

void BufferPlayer::trigger(std::string name, float value)
{
    if (name == SIGNAL_SAMPLER_TRIGGER_SET_POSITION)
    {
        this->phase = value * this->graph->get_sample_rate();
    }
    else
    {
        throw std::runtime_error("Unknown trigger: " + name);
    }
}

}
