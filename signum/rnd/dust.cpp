#include "dust.h"

#include "stdlib.h"

#include "../util.h"

namespace signum::rnd
{

Dust::Dust(float frequency)
{
	this->frequency = frequency;
	this->steps_remaining = 0;
}

void Dust::next(int count)
{
	for (int i = 0; i < count; i++)
	{
		if (this->steps_remaining <= 0)
		{
			this->steps_remaining = rng_randint(0, 44100.0 / (this->frequency / 2.0));
		}

		this->steps_remaining--;
		this->output->data[0][i] = (this->steps_remaining == 0) ? 1 : 0;
	}
}

}
