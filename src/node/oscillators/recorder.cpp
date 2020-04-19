#include "signal/node/oscillators/recorder.h"

namespace libsignal
{

Recorder::Recorder(BufferRef buffer, NodeRef input, bool loop)
    : buffer(buffer), input(input), loop(loop)
{
    this->name = "recorder";

    this->add_input("input", this->input);

    this->phase = 0.0;

    this->num_input_channels = buffer->num_channels;
    this->num_output_channels = 0;

    this->min_input_channels = this->max_input_channels = this->num_input_channels;
    this->min_output_channels = this->max_output_channels = 0;
}

void Recorder::process(sample **out, int num_frames)
{
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
