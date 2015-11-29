#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class Tick : public Unit
	{
	public:
		Tick(UnitRef frequency) :
			frequency(frequency)
		{
			this->steps_remaining = 0;
			this->name = "tick";
			this->add_param("frequency", this->frequency);
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				sample rv = 0;
				if (this->steps_remaining <= 0)
				{
					this->steps_remaining = this->graph->sample_rate / this->frequency->out[0][frame];
					rv = 1;
				}
				else
				{
					this->steps_remaining--;
				}
				for (int channel = 0; channel < this->channels_out; channel++)
				{
					this->out[channel][frame] = rv;
				}
			}
		}

		UnitRef frequency;
		int steps_remaining;

	};
}
