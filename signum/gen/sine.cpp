#include "sine.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

SineWave::SineWave(int frequency) : Unit()
{
	this->frequency = frequency;
	this->phase = 0;
}

void SineWave::next(Buffer &buffer, int count)
{
	for (int i = 0; i < count; i++)
	{
		buffer.data[0][i] = sin(this->phase++ * this->frequency * M_PI * 2.0 / 44100.0);
	}
}

}
