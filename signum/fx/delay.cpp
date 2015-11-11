#include "delay.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

void Delay::next(int count)
{
	input->next(count);

	this->buffer.extend(input->output->data[0], count);

	int offset = this->delaytime * signum_samplerate();
	printf("offset: %d\n", offset);

	for (int i = 0; i < count; i++)
	{
		sample s = this->input->output->data[0][i] + this->feedback * this->output->data[0][i - offset];
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
