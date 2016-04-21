#include "eq.h" 

#include "../gen/constant.h"
#include "../graph.h"

#include <stdlib.h>

namespace libsignal
{

void EQ::next(sample **out, int num_frames)
{
	float low, mid, high;

	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			float lf = 2 * sin(M_PI * ((double) this->low_freq->out[channel][frame] / this->graph->sample_rate));
			float hf = 2 * sin(M_PI * ((double) this->high_freq->out[channel][frame] / this->graph->sample_rate));

			/*------------------------------------------------------------------------
			 * Low-pass filter
			 *-----------------------------------------------------------------------*/
			float sample = this->input->out[channel][frame];
			this->f1p0[channel]  += (lf * (sample   - this->f1p0[channel]));
			this->f1p1[channel]  += (lf * (this->f1p0[channel] - this->f1p1[channel]));
			this->f1p2[channel]  += (lf * (this->f1p1[channel] - this->f1p2[channel]));
			this->f1p3[channel]  += (lf * (this->f1p2[channel] - this->f1p3[channel]));
			low = this->f1p3[channel];

			/*------------------------------------------------------------------------
			 * High-pass filter
			 *-----------------------------------------------------------------------*/
			this->f2p0[channel]  += (hf * (sample   - this->f2p0[channel]));
			this->f2p1[channel]  += (hf * (this->f2p0[channel] - this->f2p1[channel]));
			this->f2p2[channel]  += (hf * (this->f2p1[channel] - this->f2p2[channel]));
			this->f2p3[channel]  += (hf * (this->f2p2[channel] - this->f2p3[channel]));
			high = this->sdm3[channel] - this->f2p3[channel];

			/*------------------------------------------------------------------------
			 * Midrange (signal - (low + high))
			 *-----------------------------------------------------------------------*/
			mid = this->sdm3[channel] - (high + low);

			low *= this->low_gain->out[channel][frame];
			mid *= this->mid_gain->out[channel][frame];
			high *= this->high_gain->out[channel][frame];

			this->sdm3[channel] = this->sdm2[channel];
			this->sdm2[channel] = this->sdm1[channel];
			this->sdm1[channel] = sample;                

			this->out[channel][frame] = low + mid + high;
		}
	}
}

}
