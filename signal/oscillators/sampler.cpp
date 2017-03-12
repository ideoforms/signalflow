#include "sampler.h" 
#include "constant.h"
#include "../graph.h"

#include <stdlib.h>

namespace libsignal
{

Sampler::Sampler(BufferRef buffer, NodeRef rate, PropertyRef loop) : rate(rate), loop(loop)
{
	this->name = "sampler";

	this->add_input("rate", this->rate);
	this->add_property("loop", this->loop);

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
	for (int frame = 0; frame < num_frames; frame++)
	{
		for (int channel = 0; channel < this->num_output_channels; channel++)
		{
			if ((int) this->phase < buffer->num_frames)
			{
				s = this->buffer->data[channel][(int) this->phase];
			}
			else
			{
				if (loop->int_value())
				{
					this->phase = 0;
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
	if (name == SIGNAL_DEFAULT_TRIGGER)
	{
		this->phase = value * this->graph->sample_rate;
	}
}

}
