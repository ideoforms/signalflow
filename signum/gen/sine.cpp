#include "sine.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

void Sine::next(int count)
{
	this->frequency->next(count);

	for (int i = 0; i < count; i++)
	{
		this->output->data[0][i] = sin(this->phase++ * this->frequency[i] * M_PI * 2.0 / 44100.0);
	}
}

}
