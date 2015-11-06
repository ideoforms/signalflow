#include "square.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

SquareWave::SquareWave(int frequency, float width) : Unit()
{
	this->frequency = frequency;
	this->width = width;
	this->phase = 0.0;
}

void SquareWave::next(int count)
{
	for (int i = 0; i < count; i++)
	{
		this->output->data[0][i] = (this->phase < this->width) ? 1 : -1;

		this->phase += 1.0 / (44100.0 / this->frequency);
		if (this->phase >= 1.0)
			this->phase -= 1.0;
	}
}

}
