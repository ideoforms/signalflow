#include "resample.h" 

#include "../gen/constant.h"
#include "../graph.h"

#include <stdlib.h>

namespace libsignal
{

void Resample::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		float phase_increment = this->sample_rate->out[0][frame] / this->graph->sample_rate;

		for (int channel = 0; channel < num_output_channels; channel++)
		{
			out[channel][frame] = sample_last[channel];
			if (int(phase) > int(phase_last))
				sample_last[channel] = this->input->out[channel][frame];
			phase_last = phase;
		}

		phase += phase_increment;
	}
}

}
