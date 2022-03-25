#include "signalflow/node/buffer/buffer-looper.h"

namespace signalflow
{

BufferLooper::BufferLooper(BufferRef buffer, NodeRef input, NodeRef feedback,
                           bool loop_playback, bool loop_record)
    : buffer(buffer), input(input), feedback(feedback), loop_playback(loop_playback), loop_record(loop_record)
{
    SIGNALFLOW_CHECK_GRAPH()

    this->name = "buffer-looper";

    this->create_buffer("buffer", this->buffer);
    this->create_input("input", this->input);
    this->create_input("feedback", this->feedback);

    if (buffer)
    {
        /*--------------------------------------------------------------------------------
         * buffer can be null when a BufferPlayer is being instantiated as part of
         * a Patch. Its buffer is populated later.
         *--------------------------------------------------------------------------------*/
        this->set_buffer("buffer", buffer);
    }

    this->phase = 0.0;
    this->is_playing = false;
    this->is_recording = false;
}

void BufferLooper::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        this->phase = 0.0;
    }
    else if (name == SIGNALFLOW_TRIGGER_PLAY)
    {
        if (this->state = SIGNALFLOW_NODE_STATE_STOPPED)
        {
            this->phase = 0.0;
        }
        this->is_playing = true;
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
    else if (name == SIGNALFLOW_TRIGGER_RECORD)
    {
        if (this->state = SIGNALFLOW_NODE_STATE_STOPPED)
        {
            this->phase = 0.0;
        }
        this->is_recording = true;
        this->state = SIGNALFLOW_NODE_STATE_ACTIVE;
    }
    else if (name == SIGNALFLOW_TRIGGER_SET_POSITION)
    {
        this->phase = buffer->get_sample_rate() * value;
    }
}

void BufferLooper::process(Buffer &out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->get_num_frames())
        return;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_input_channels; channel++)
        {
            this->out[channel][frame] = 0.0;
            if ((unsigned int) this->phase < buffer->get_num_frames())
            {
                if (this->is_recording)
                {
                    this->buffer->data[channel][(int) this->phase] = (this->feedback->out[channel][frame] * this->buffer->data[channel][(int) this->phase])
                        + this->input->out[channel][frame];
                }
                if (this->is_playing)
                {
                    this->out[channel][frame] = this->buffer->data[channel][(int) this->phase];
                }
            }
        }

        this->phase += 1;
        if (this->phase >= this->buffer->get_num_frames())
        {
            if (loop_playback || loop_record)
            {
                while (this->phase >= this->buffer->get_num_frames())
                    this->phase -= this->buffer->get_num_frames();
                if (!loop_playback)
                {
                    this->is_playing = false;
                }
                if (!loop_record)
                {
                    this->is_recording = false;
                }
            }
            else
            {
                this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
                this->is_playing = false;
                this->is_recording = false;
            }
        }
    }
}

}
