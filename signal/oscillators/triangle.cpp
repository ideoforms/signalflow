#include "triangle.h"
#include "../graph.h"

namespace libsignal
{

void Triangle::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			float rv = (this->phase[channel] < 0.5) ? (this->phase[channel] * 4.0 - 1.0) : (1.0 - (this->phase[channel] - 0.5) * 4.0);

			out[channel][frame] = rv;

			this->phase[channel] += this->frequency->out[channel][frame] / this->graph->sample_rate;
			while (this->phase[channel] >= 1.0)
				this->phase[channel] -= 1.0;
		}
	}
}

}
