#include "noise.h"

#include "stdlib.h"
#include "../util.h"
#include "../graph.h"

#include <limits>

namespace libsignal
{

Noise::Noise(NodeRef frequency, bool interpolate, NodeRef min, NodeRef max) : frequency(frequency), min(min), max(max)
{
	this->name = "noise";
	this->add_param("frequency", this->frequency);
	this->add_param("min", this->min);
	this->add_param("max", this->max);

	this->interpolate = interpolate;

	this->value = std::numeric_limits<float>::max();

	this->steps_remaining = 0;
}

void Noise::next(sample **out, int num_frames)
{
	if (this->value == std::numeric_limits<float>::max())
	{
		// TODO: Put this in an init block that is available to all
		// nodes on their first block?
		this->value = this->min->out[0][0];
	}

	for (int frame = 0; frame < num_frames; frame++)
	{
		float min = this->min->out[0][frame];
		float max = this->max->out[0][frame];
		float frequency = this->frequency->out[0][frame];
		if (!frequency)
			frequency = this->graph->sample_rate;

		if (this->steps_remaining <= 0)
		{
			// pick a new target value
			float target = random_uniform(min, max);

			if (frequency > 0)
			{
				this->steps_remaining = random_integer(0, this->graph->sample_rate / (frequency / 2.0));
				if (this->steps_remaining == 0)
					this->steps_remaining = 1;
				this->step_change = (target - value) / this->steps_remaining;
			}
			else
			{
				this->steps_remaining = 0;
				this->step_change = target - value;
			}

			if (!this->interpolate)
			{
				this->value = target;
				this->step_change = 0;
			}
		}

		this->value += this->step_change;

		for (int channel = 0; channel < this->num_output_channels; channel++)
			out[channel][frame] = this->value;

		this->steps_remaining--;
	}
}

}
