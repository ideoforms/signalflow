#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/buffer-player.h"

#include <limits>
#include <stdlib.h>

namespace signalflow
{

#define SIGNALFLOW_BUFFER_PLAYER_NULL_PHASE std::numeric_limits<int>::max()

BufferPlayer::BufferPlayer(BufferRef buffer, NodeRef rate, NodeRef loop,
                           NodeRef start_time, NodeRef end_time, NodeRef clock)
    : rate(rate), loop(loop), start_time(start_time), end_time(end_time), clock(clock)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "buffer-player";

    this->create_buffer("buffer", this->buffer);
    this->set_channels(1, 0);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a BufferPlayer is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }

    this->create_input("rate", this->rate);
    this->create_input("loop", this->loop);
    this->create_input("start_time", this->start_time);
    this->create_input("end_time", this->end_time);
    this->create_input("clock", this->clock);

    /*--------------------------------------------------------------------------------
     * In cases where a clock is set, we don't immediately want to begin playback,
     * so initially set phase to the largest possible value.
     *
     * Although phase needs to support fractional values, it is rounded as an int
     * offset so int range should be used.
     *--------------------------------------------------------------------------------*/
    this->phase = SIGNALFLOW_BUFFER_PLAYER_NULL_PHASE;
    this->state = SIGNALFLOW_NODE_STATE_STOPPED;
    this->loop_direction = 1;

    if (!clock)
    {
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
}

void BufferPlayer::set_buffer(std::string name, BufferRef buffer)
{
    if (name == "buffer")
    {
        this->Node::set_buffer(name, buffer);
        this->num_output_channels = buffer->get_num_channels();
        this->rate_scale_factor = buffer->get_sample_rate() / graph->get_sample_rate();
    }
}

void BufferPlayer::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        /*----------------------------------------------------------------
         * Set the offset within the buffer, in samples.
         *----------------------------------------------------------------*/
        if (this->start_time)
        {
            this->phase = buffer->get_sample_rate() * this->start_time->out[0][0];
        }
        else
        {
            this->phase = 0.0;
        }
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
    else if (name == SIGNALFLOW_TRIGGER_SET_POSITION)
    {
        this->phase = buffer->get_sample_rate() * value;
    }
    else if (name == SIGNALFLOW_TRIGGER_STOP)
    {
        this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

void BufferPlayer::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    sample s;

    int start_frame = this->start_time ? (buffer->get_sample_rate() * this->start_time->out[0][0]) : 0;
    int end_frame = this->end_time ? (buffer->get_sample_rate() * this->end_time->out[0][0]) : buffer->get_num_frames();

    if (this->phase == SIGNALFLOW_BUFFER_PLAYER_NULL_PHASE)
    {
        /*--------------------------------------------------------------------------------
         * Starting playback.
         * Begin at the start or end of the loop as appropriate.
         *--------------------------------------------------------------------------------*/
        if (this->rate->out[0][0] > 0)
        {
            this->phase = start_frame;
        }
        else
        {
            this->phase = end_frame;
        }
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        float rate = this->rate->out[0][frame];

        if (SIGNALFLOW_CHECK_TRIGGER(this->clock, frame))
        {
            this->trigger();
        }
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            if (this->state == SIGNALFLOW_NODE_STATE_STOPPED)
            {
                s = 0.0;
            }
            else
            {
                s = this->buffer->get_frame(channel, phase);
            }

            out[channel][frame] = s;
        }

        this->phase += this->loop_direction * rate * this->rate_scale_factor;

        if (this->phase < start_frame || (int) this->phase >= end_frame)
        {
            // Player is beyond the start or end of the buffer
            int loop = this->loop->out[0][frame];
            if (loop == 0)
            {
                if (this->state == SIGNALFLOW_NODE_STATE_ACTIVE)
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }
            else if (loop == 1)
            {
                // This should be set to correct the loop dir in case we have transitioned from loop == 2
                this->loop_direction = 1;

                if (rate < 0)
                {
                    this->phase = end_frame - 1;
                }
                else if (rate > 0)
                {
                    this->phase = start_frame;
                }
                else
                {
                    // shouldn't happen
                }
            }
            else if (loop == 2)
            {
                if (this->phase < start_frame)
                {
                    this->phase = start_frame;
                    this->loop_direction = rate > 0 ? 1 : -1;
                }
                else
                {
                    this->phase = end_frame - 1;
                    this->loop_direction = rate > 0 ? -1 : 1;
                }
            }
        }
    }
}

PropertyRef BufferPlayer::get_property(std::string name)
{
    if (name == "position")
    {
        /*--------------------------------------------------------------------------------
         * Return the current playback position, in samples.
         * Important to account for rate_scale_factor, if buffer is being played back
         * on an AudioGraph with a different sample rate to that of its own.
         *--------------------------------------------------------------------------------*/
        if (this->phase == SIGNALFLOW_BUFFER_PLAYER_NULL_PHASE)
        {
            return 0.0;
        }
        else
        {
            return this->phase / (this->graph->get_sample_rate() * this->rate_scale_factor);
        }
    }
    else
    {
        return Node::get_property(name);
    }
}

}
