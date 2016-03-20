#pragma once 

#include "../unit.h"

namespace libsignal
{
	class Tick : public Unit
	{
	public:
		Tick(UnitRef frequency = 1.0) :
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

				this->out[0][frame] = rv;
			}
		}

		UnitRef frequency;
		int steps_remaining;

	};

	REGISTER(Tick, "tick");
}
