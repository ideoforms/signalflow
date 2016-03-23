#include "eq.h" 

#include "../gen/constant.h"

#include <stdlib.h>

namespace libsignal
{

void EQ::next(sample **out, int num_frames)
{
	this->lf = 2 * sin(M_PI * ((double) this->low_freq->out[0][0] / this->graph->sample_rate));
	this->hf = 2 * sin(M_PI * ((double) this->high_freq->out[0][0] / this->graph->sample_rate));

	float low, mid, high;

	for (int frame = 0; frame < num_frames; frame++)
	{
		/*------------------------------------------------------------------------
		 * Low-pass filter
		 *-----------------------------------------------------------------------*/
		float sample = this->input->out[0][frame];
		this->f1p0  += (this->lf * (sample   - this->f1p0));
		this->f1p1  += (this->lf * (this->f1p0 - this->f1p1));
		this->f1p2  += (this->lf * (this->f1p1 - this->f1p2));
		this->f1p3  += (this->lf * (this->f1p2 - this->f1p3));
		low = this->f1p3;

		/*------------------------------------------------------------------------
		 * High-pass filter
		 *-----------------------------------------------------------------------*/
		this->f2p0  += (this->hf * (sample   - this->f2p0));
		this->f2p1  += (this->hf * (this->f2p0 - this->f2p1));
		this->f2p2  += (this->hf * (this->f2p1 - this->f2p2));
		this->f2p3  += (this->hf * (this->f2p2 - this->f2p3));
		high = this->sdm3 - this->f2p3;

		/*------------------------------------------------------------------------
		 * Midrange (signal - (low + high))
		 *-----------------------------------------------------------------------*/
		mid = this->sdm3 - (high + low);

		low *= this->low_gain->out[0][frame];
		mid *= this->mid_gain->out[0][frame];
		high *= this->high_gain->out[0][frame];

		this->sdm3 = this->sdm2;
		this->sdm2 = this->sdm1;
		this->sdm1 = sample;                

		for (int channel = 0; channel < channels_out; channel++)
		{
			this->out[channel][frame] = low + mid + high;
		}
	}
}

}
