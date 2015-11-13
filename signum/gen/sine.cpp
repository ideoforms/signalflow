#include "sine.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

sample Sine::next()
{
	return sin(this->phase++ * this->frequency->next() * M_PI * 2.0 / 44100.0);
}

}
