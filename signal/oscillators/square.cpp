#include "square.h"
#include "../graph.h"

namespace libsignal
{

void Square::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			float frequency = this->frequency->out[channel][frame];
			float width = this->width->out[channel][frame];
			float rv = (this->phase[channel] < width) ? 1 : -1;

			out[channel][frame] = rv;

			this->phase[channel] += 1.0 / (this->graph->sample_rate / frequency);
			if (this->phase[channel] >= 1.0)
				this->phase[channel] -= 1.0;
		}
	}
}

}
