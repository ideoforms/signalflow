#include "delay.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

sample Delay::next()
{
	float d = this->delaytime->next();
	float f = this->feedback->next();
	int offset = d * signum_samplerate();

	sample rv = this->input->next() + f * buffer.get(-offset);
	buffer.append(rv);

	return rv;
}

/*
void Delay::_next()
{
	int index = this->delaytime * signum_samplerate();
	sample output = this->input->output[0] + this->feedback * this->output[-index];
}
*/

}
