#include "signalflow/node/buffer/buffer-recorder.h"

namespace signalflow
{

BufferRecorder::BufferRecorder(BufferRef buffer, NodeRef input, NodeRef feedback, bool loop)
    : buffer(buffer), input(input), feedback(feedback), loop(loop)
{
    if (!buffer)
    {
        throw std::runtime_error("No buffer specified");
    }

    this->name = "buffer-recorder";

    this->create_buffer("buffer", this->buffer);
    this->create_input("input", this->input);
    this->create_input("feedback", this->feedback);

    this->phase = 0.0;

    this->set_channels(buffer->get_num_channels(), 0);
}

void BufferRecorder::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        this->set_state(SIGNALFLOW_NODE_STATE_ACTIVE);
        this->phase = 0.0;
    }
    else if (name == SIGNALFLOW_TRIGGER_SET_POSITION)
    {
        this->phase = buffer->get_sample_rate() * value;
    }
}

void BufferRecorder::process(Buffer &out, int num_frames)
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
            if ((unsigned int) this->phase < buffer->get_num_frames())
            {
                this->buffer->data[channel][(int) this->phase] = (this->feedback->out[channel][frame] * this->buffer->data[channel][(int) this->phase])
                    + this->input->out[channel][frame];
            }
        }

        this->phase += 1;
        if (this->phase >= this->buffer->get_num_frames())
        {
            if (loop)
            {
                while (this->phase >= this->buffer->get_num_frames())
                    this->phase -= this->buffer->get_num_frames();
            }
            else
            {
                this->set_state(SIGNALFLOW_NODE_STATE_STOPPED);
            }
        }
    }
}

}
