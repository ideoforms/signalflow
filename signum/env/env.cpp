#include "env.h"

namespace signum::env
{

ASR::ASR(float attack, float sustain, float release, UnitRef clock) : clock(clock)
{
	this->attack = attack;
	this->sustain = sustain;
	this->release = release;
	this->phase = 0.0;

	this->add_param("clock", this->clock);
}

void ASR::trigger()
{
	this->phase = 0.0;
}

void ASR::next(int count)
{
	sample *ptr = this->output->data[0];

	if (this->clock)
		this->clock->next(count);

	float clock_last = 0.0;

	for (int i = 0; i < count; i++)
	{
		if (this->clock)
		{
			sample clock_value = this->clock->output->data[0][i];
			if (clock_value > clock_last)
				this->trigger();
			clock_last = clock_value;
		}

		if (this->phase < this->attack)
		{
			*ptr = (this->phase / this->attack);
		}
		else if (this->phase <= this->attack + this->sustain)
		{
			*ptr = 1.0;
		}
		else if (this->phase < this->attack + this->sustain + this->release)
		{
			*ptr = 1.0 - (this->phase - (this->attack + this->sustain)) / this->release;
		}
		else
		{
			*ptr = 0.0;
		}

		this->phase += 1.0 / 44100.0;
		ptr++;
	}
}

}
