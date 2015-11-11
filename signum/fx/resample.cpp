#include "resample.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

void Resample::next(int count)
{
	input->next(count);
	sample_rate->next(count);
	bit_rate->next(count);

	float phase_increment = this->sample_rate->output->data[0][0] / signum_samplerate();

	for (int i = 0; i < count; i++)
	{
		phase += phase_increment;
		if (int(phase) > int(phase_last))
			sample_last = this->input->output->data[0][i];
		phase_last = phase;

		this->output->data[0][i] = sample_last;
	}
}

}
