#include "sine.h"
#include "../signum.h"

#include <math.h>
#include <stdio.h>

namespace signum::gen
{

void Sine::next(sample **out, int num_frames)
{
	// printf("Sine::next (phase %d, %d, %d)\n", this->phase, num_frames, this->channels_out);
	for (int frame = 0; frame < num_frames; frame++)
	{
		float freq = this->frequency->out[0][frame];
		for (int channel = 0; channel < this->channels_out; channel++)
		{
			out[0][frame] = sin(this->phase * freq * M_PI * 2.0 / signum_samplerate()) * 0.3;
		}
		this->phase++;
	}
}

}
