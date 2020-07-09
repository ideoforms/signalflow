#include "signal/node/buffer/buffer-recorder.h"

namespace libsignal
{

BufferRecorder::BufferRecorder(BufferRef buffer, NodeRef input, bool loop)
    : buffer(buffer), input(input), loop(loop)
{
    if (!buffer)
    {
        throw std::runtime_error("No buffer specified");
    }

    this->name = "buffer-recorder";

    this->add_buffer("buffer", this->buffer);
    this->add_input("input", this->input);

    this->phase = 0.0;

    this->num_input_channels = buffer->num_channels;
    this->num_output_channels = 0;

    this->min_input_channels = this->max_input_channels = this->num_input_channels;
    this->min_output_channels = this->max_output_channels = 0;
}

void BufferRecorder::process(sample **out, int num_frames)
{
    /*--------------------------------------------------------------------------------
     * If buffer is null or empty, don't try to process.
     *--------------------------------------------------------------------------------*/
    if (!this->buffer || !this->buffer->num_frames)
        return;

    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_input_channels; channel++)
        {
            if ((int) this->phase < buffer->num_frames)
            {
                this->buffer->data[channel][(int) this->phase] = this->input->out[channel][frame];
            }
        }

        this->phase += 1;
        if (loop)
        {
            while (this->phase > this->buffer->num_frames)
            {
                this->phase -= this->buffer->num_frames;
            }
        }
    }
}

}
