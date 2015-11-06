#include "noise.h"

#include "stdlib.h"


namespace signum::gen
{

Noise::Noise(float frequency, bool interpolate)
{
	this->frequency = frequency;
	this->interpolate = interpolate;
	this->value = (float) rand() / RAND_MAX;

	this->steps_remaining = 0;
}

void Noise::next(int count)
{
	for (int i = 0; i < count; i++)
	{
		if (this->steps_remaining <= 0)
		{
			// pick a new target value
			float target = (float) rand() / RAND_MAX;

			if (this->frequency > 0)
			{
				this->steps_remaining = rand() % (int) (44100.0 / this->frequency);
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

		this->output->data[0][i] = this->value;
		this->steps_remaining--;
	}
}

}
