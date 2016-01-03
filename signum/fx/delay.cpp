#include "delay.h" 

#include "../gen/constant.h"

#include <stdlib.h>

namespace signum
{

void Delay::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		sample d = this->delaytime->out[0][frame];
		sample f = this->feedback->out[0][frame];
		int offset = d * this->graph->sample_rate;

		for (int channel = 0; channel < this->channels_in; channel++)
		{
			sample rv = input->out[channel][frame] + f * buffers[channel]->get(-offset);
			out[channel][frame] = rv;
			buffers[channel]->append(rv);
			// TODO should copy bulk samples rather than appending single
		}
	}
}

}
