#include "delay.h" 
#include "../gen/constant.h"
#include "../signum.h" 

#include <stdlib.h>

namespace signum::fx
{

void Delay::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample d = this->delaytime->out[0][frame];
		sample f = this->feedback->out[0][frame];
		int offset = d * signum_samplerate();

		for (int channel = 0; channel < this->channels_in; channel++)
		{
			sample rv = this->input->out[channel][frame] + f * buffers[frame].get(-offset);
			out[channel][frame] = rv;
		}
	}
}

}
