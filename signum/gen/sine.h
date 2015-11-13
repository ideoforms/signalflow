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
		}

		UnitRef frequency;
		int phase;

		virtual void next(int count);
	};
}
