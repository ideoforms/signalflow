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

	return this->input->next() + f * this->output->data[0][-offset];
}

/*
void Delay::_next()
{
	int index = this->delaytime * signum_samplerate();
	sample output = this->input->output[0] + this->feedback * this->output[-index];
}
*/

}
