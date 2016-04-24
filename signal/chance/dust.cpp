#include "dust.h"

#include "stdlib.h"

#include "../util.h"
#include "../graph.h"

namespace libsignal
{

Dust::Dust(NodeRef frequency) : frequency(frequency)
{
	this->steps_remaining = 0;

	this->name = "dust";
	this->add_param("frequency", this->frequency);
}

void Dust::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		float freq = this->frequency->out[0][frame];

		if (this->steps_remaining <= 0)
		{
			this->steps_remaining = random_integer(0, this->graph->sample_rate / (freq / 2.0));
		}
		this->steps_remaining--;

		for (int channel = 0; channel < this->num_output_channels; channel++)
		{
			out[channel][frame] = (this->steps_remaining == 0) ? 1 : 0;
		}

	}
}

}
