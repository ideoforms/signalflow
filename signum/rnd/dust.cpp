#include "dust.h"

#include "stdlib.h"

#include "../util.h"

namespace signum::rnd
{

Dust::Dust(UnitRef frequency) // : frequency(frequency)
{
	this->frequency = frequency;
	this->steps_remaining = 0;
}

sample Dust::next()
{
	float f = this->frequency->next();

	if (this->steps_remaining <= 0)
		this->steps_remaining = rng_randint(0, 44100.0 / (f / 2.0));

	this->steps_remaining--;
	return (this->steps_remaining == 0) ? 1 : 0;
}

}
