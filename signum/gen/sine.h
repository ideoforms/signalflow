#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class Sine : public Unit
	{
	public:
		Sine(UnitRef frequency) : frequency(frequency)
		{
			this->phase = 0;
			this->name = "sine";
			this->add_param("frequency", this->frequency);
		}

		UnitRef frequency;
		int phase;

		virtual void next(sample **out, int num_frames);
	};
}
