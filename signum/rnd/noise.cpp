#include "noise.h"

#include "stdlib.h"
#include "../util.h"

namespace signum::rnd
{

Noise::Noise(UnitRef frequency, bool interpolate, float min, float max) : frequency(frequency)
{
	this->interpolate = interpolate;
	this->min = min;
	this->max = max;

	this->value = rng_uniform(min, max);

	this->steps_remaining = 0;
}

void Noise::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		float frequency = this->frequency->out[0][0];

		if (this->steps_remaining <= 0)
		{
			// pick a new target value
			float target = rng_uniform(min, max);

			if (frequency > 0)
			{
				this->steps_remaining = rng_randint(0, 44100.0 / (frequency / 2.0));
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

		for (int channel = 0; channel < this->channels_out; channel++)
			out[channel][frame] = this->value;

		this->steps_remaining--;
	}
}

}
