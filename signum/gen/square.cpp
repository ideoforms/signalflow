#include "square.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

sample Square::next()
{
	float s = (this->phase < this->width->next()) ? 1 : -1;

	this->phase += 1.0 / (44100.0 / this->frequency->next());
	if (this->phase >= 1.0)
		this->phase -= 1.0;

	return s;
}

}
