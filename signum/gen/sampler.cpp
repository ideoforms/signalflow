#include "sampler.h" 
#include "constant.h"

#include <stdlib.h>

namespace signum
{

Sampler::Sampler(Buffer *buffer, UnitRef rate, bool loop) : rate(rate)
{
	this->name = "sampler";

	this->add_param("rate", this->rate);

	this->loop = loop;
	this->phase = 0.0;

	this->buffer = buffer;

	this->channels_in = 0;
	this->channels_out = buffer->num_channels;

	this->min_input_channels = this->max_input_channels = 0;
	this->min_output_channels = this->max_output_channels = this->channels_out;

	this->trigger();
}

void Sampler::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample s;
		for (int channel = 0; channel < this->channels_out; channel++)
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

void Sampler::trigger()
{
	this->phase = 0;
}

}
