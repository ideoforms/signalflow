#pragma once 

#include "../unit.h"

namespace libsignal
{
	class Sine : public Unit
	{
	public:
		Sine(UnitRef frequency = 440) : frequency(frequency)
		{
			this->phase = 0;
			this->name = "sine";
			this->add_param("frequency", this->frequency);
		}

		UnitRef frequency;
		float phase;

		virtual void next(sample **out, int num_frames);
	};

	REGISTER(Sine, "sine");

}

