#include "signalflow/core/core.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/buffer/buffer-looper.h"

namespace signalflow
{

BufferLooper::BufferLooper(BufferRef buffer, NodeRef input, NodeRef feedback,
                           NodeRef loop_playback, NodeRef loop_record,
                           NodeRef start_time, NodeRef end_time,
                           NodeRef looper_level, NodeRef playthrough_level)
    : buffer(buffer), input(input), feedback(feedback), loop_playback(loop_playback), loop_record(loop_record), start_time(start_time), end_time(end_time), looper_level(looper_level), playthrough_level(playthrough_level)
{
    SIGNALFLOW_CHECK_GRAPH()

    this->name = "buffer-looper";

    this->create_buffer("buffer", this->buffer);
    this->create_input("input", this->input);
    this->create_input("feedback", this->feedback);
    this->create_input("loop_playback", this->loop_playback);
    this->create_input("loop_record", this->loop_record);
    this->create_input("start_time", this->start_time);
    this->create_input("end_time", this->end_time);
    this->create_input("looper_level", this->looper_level);
    this->create_input("playthrough_level", this->playthrough_level);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a BufferPlayer is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }

    this->playback_phase = 0.0;
    this->record_phase = 0.0;
    this->is_playing = false;
    this->is_recording = false;
}

void BufferLooper::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        if (start_time)
            this->playback_phase = (int) this->start_time->out[0][0] * this->graph->get_sample_rate();
        else
            this->playback_phase = 0;
    }
    else if (name == SIGNALFLOW_TRIGGER_PLAY)
    {
        if (start_time)
            this->playback_phase = (int) this->start_time->out[0][0] * this->graph->get_sample_rate();
        else
            this->playback_phase = 0;
        this->is_playing = true;
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
    else if (name == SIGNALFLOW_TRIGGER_RECORD)
    {
        this->record_phase = 0.0;
        this->is_recording = true;
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
    else if (name == SIGNALFLOW_TRIGGER_SET_POSITION)
    {
        this->playback_phase = buffer->get_sample_rate() * value;
    }
    else
    {
        this->Node::trigger(name, value);
    }
}

void BufferLooper::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;
    if (this->buffer->get_num_channels() != (unsigned int) this->num_input_channels)
    {
        signalflow_audio_thread_error("BufferLooper: Buffer channel count does not match the audio input (" + std::to_string(this->buffer->get_num_channels()) + " != " + std::to_string(this->num_input_channels) + ")");
    }

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_input_channels; channel++)
        {
            this->out[channel][frame] = this->input->out[channel][frame] * this->playthrough_level->out[0][frame];

            if ((unsigned int) this->playback_phase < buffer->get_num_frames())
            {
                if (this->is_playing)
                {
                    this->out[channel][frame] += this->buffer->data[channel][(int) this->playback_phase] * this->looper_level->out[channel][frame];
                }
            }
            if ((unsigned int) this->record_phase < buffer->get_num_frames())
            {
                if (this->is_recording)
                {
                    this->buffer->data[channel][(int) this->record_phase] = (this->feedback->out[channel][frame] * this->buffer->data[channel][(int) this->record_phase])
                        + this->input->out[channel][frame];
                }
            }
        }

        this->playback_phase += 1;
        this->record_phase += 1;
        int last_frame = this->end_time ? (this->end_time->out[0][frame] * this->graph->get_sample_rate()) : this->buffer->get_num_frames();
        if (this->playback_phase >= last_frame)
        {
            if (loop_playback->out[0][frame])
            {
                if (this->start_time)
                    this->playback_phase = this->start_time->out[0][frame] * this->graph->get_sample_rate();
                else
                    this->playback_phase = 0;
            }
            else
            {
                this->is_playing = false;
                if (!this->is_recording)
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }
        }
        if (this->record_phase >= this->buffer->get_num_frames())
        {
            if (loop_record->out[0][frame])
            {
                this->record_phase = 0;
            }
            else
            {
                this->is_recording = false;
                if (!this->is_playing)
                {
                    this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                }
            }
        }
    }
}

}
