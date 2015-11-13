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

sample Sampler::next()
{
		sample s;
		if ((int) this->phase < buffer->num_frames)
		{
			s = this->buffer->data[0][(int) this->phase];
		}
		else
		{
			if (loop)
			{
				this->phase = 0;
				s = this->buffer->data[0][(int) this->phase];
			}
			else
			{
				s = 0.0;
			}
		}

		// printf("sample: %f\n", s);
		this->phase += this->rate->next();

		return s;
}

void Sampler::trigger()
{
	this->phase = 0;
}

}
