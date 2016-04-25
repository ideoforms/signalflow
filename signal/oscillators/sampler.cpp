#include "sampler.h" 
#include "constant.h"
#include "../graph.h"

#include <stdlib.h>

namespace libsignal
{

Sampler::Sampler(Buffer *buffer, NodeRef rate, bool loop) : rate(rate)
{
	this->name = "sampler";

	this->add_input("rate", this->rate);

	this->loop = loop;
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
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample s;
		for (int channel = 0; channel < this->num_output_channels; channel++)
		{
			if ((int) this->phase < buffer->num_frames)
			{
				s = this->buffer->data[channel][(int) this->phase];
			}
			else
			{
				if (loop)
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
