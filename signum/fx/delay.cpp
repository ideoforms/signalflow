#include "delay.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

void Delay::next(int count)
{
	input->next(count);

	delaytime->next(count);
	feedback->next(count);

	this->buffer.extend(input->output->data[0], count);

	float d = this->delaytime->output->data[0][0];
	int offset = d * signum_samplerate();
	printf("offset: %d\n", offset);

	for (int i = 0; i < count; i++)
	{
		float f = this->feedback->output->data[0][i];
		sample s = this->input->output->data[0][i] + f * this->output->data[0][i - offset];
		this->output->data[0][i] = s;
	}
}

/*
void Delay::_next()
{
	int index = this->delaytime * signum_samplerate();
	sample output = this->input->output[0] + this->feedback * this->output[-index];
}
*/

}
