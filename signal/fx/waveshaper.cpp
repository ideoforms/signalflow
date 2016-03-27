#include "waveshaper.h" 

#include "../gen/constant.h"

#include <stdlib.h>

namespace libsignal
{

void WaveShaper::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			out[channel][frame] = this->input->out[channel][frame];
		}
	}
}

}

