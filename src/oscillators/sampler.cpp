#include "signal/oscillators/sampler.h"
#include "signal/graph.h"

#include <stdlib.h>

namespace libsignal
{

Sampler::Sampler(BufferRef buffer, NodeRef rate, NodeRef loop) : rate(rate), loop(loop)
{
    this->name = "sampler";

    this->add_input("rate", this->rate);
    this->add_input("loop_start", this->loop_start);
    this->add_input("loop_end", this->loop_end);
    this->add_input("loop", this->loop);

    this->phase = 0.0;

    this->buffer = buffer;

    this->num_input_channels = 0;
    this->num_output_channels = buffer->num_channels;

    this->min_input_channels = this->max_input_channels = 0;
    this->min_output_channels = this->max_output_channels = this->num_output_channels;

    this->trigger();
}

void Sampler::process(sample **out, int num_frames)
{
    sample s;

    int loop_start = this->loop_start ? (buffer->num_frames * this->loop_start->out[0][0]) : 0;
    int loop_end = this->loop_end ? (buffer->num_frames * this->loop_end->out[0][0]) : buffer->num_frames;

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
                    s = 0.0;
                }
            }

            out[channel][frame] = s;
        }

        this->phase += this->rate->out[0][frame];
    }
}

void Sampler::trigger(std::string name, float value)
{
    if (name == SIGNAL_SAMPLER_TRIGGER_SET_POSITION)
    {
        this->phase = value * this->graph->sample_rate;
    }
}

}
