#include "dust.h"

#include "stdlib.h"

#include "../util.h"

namespace signum::rnd
{

Dust::Dust(UnitRef frequency) : frequency(frequency)
{
	// this->frequency = frequency;
	this->steps_remaining = 0;

	this->name = "dust";
	this->add_param("frequency", this->frequency);
}

void Dust::next(sample **out, int num_frames)
{
	printf("got %d params\n", this->params.size());
	for (int frame = 0; frame < num_frames; frame++)
	{
		float freq = this->frequency->out[0][frame];
		printf("freq is %f\n", freq);
		if (this->steps_remaining <= 0)
			this->steps_remaining = rng_randint(0, 44100.0 / (freq / 2.0));

		for (int channel = 0; channel < this->channels_out; channel++)
		{
			out[channel][frame] = (this->steps_remaining == 0) ? 1 : 0;
		}
		this->steps_remaining--;
	}
}

}
