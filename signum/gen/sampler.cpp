#include "sampler.h" 
#include "constant.h"

#include <stdlib.h>

namespace signum::gen
{

Sampler::Sampler(Buffer *buffer, UnitRef rate, bool loop) : rate(rate)
{
	this->add_param("rate", this->rate);

	this->loop = loop;
	this->phase = 0.0;

	this->buffer = buffer;
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
