#include "env.h"

namespace signum::env
{

ASR::ASR(float attack, float sustain, float release, UnitRef clock) : clock(clock)
{
	this->attack = attack;
	this->sustain = sustain;
	this->release = release;
	this->phase = 0.0;
	this->clock_last = 0.0;

	this->name = "env-asr";
	this->add_param("clock", this->clock);
}

void ASR::trigger()
{
	this->phase = 0.0;
}

void ASR::next(sample **out, int num_frames)
{
	sample rv;

	for (int frame = 0; frame < num_frames; frame++)
	{
		if (this->clock)
		{
			sample clock_value = this->clock->out[0][frame];
			if (clock_value > this->clock_last)
				this->trigger();
			this->clock_last = clock_value;
		}

		if (this->phase < this->attack)
		{
			rv = (this->phase / this->attack);
		}
		else if (this->phase <= this->attack + this->sustain)
		{
			rv = 1.0;
		}
		else if (this->phase < this->attack + this->sustain + this->release)
		{
			rv = 1.0 - (this->phase - (this->attack + this->sustain)) / this->release;
		}
		else
		{
			rv = 0.0;
		}

		this->phase += 1.0 / 44100.0;

		for (int channel = 0; channel < this->channels_out; channel++)
		{
			out[channel][frame] = rv;
		}
	}
}

}
