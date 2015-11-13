#include "resample.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

sample Resample::next()
{
	float phase_increment = this->sample_rate->next() / signum_samplerate();
	float input = this->input->next();

	phase += phase_increment;

	if (int(phase) > int(phase_last))
		sample_last = input;
	phase_last = phase;

	return sample_last;
}

}
