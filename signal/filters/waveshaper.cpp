#include "waveshaper.h" 

#include "../oscillators/constant.h"

#include <stdlib.h>

namespace libsignal
{

void WaveShaper::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			float input = this->input->out[channel][frame];
			out[channel][frame] = this->buffer->get(input);
		}
	}
}

}

