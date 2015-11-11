#pragma once

#include "../unit.h"
#include "../constants.h"

namespace signum::fx
{
	class Resample : public Unit
	{
		public:
			Resample(UnitRef input, UnitRef sample_rate, UnitRef bit_rate) : input(input), sample_rate(sample_rate), bit_rate(bit_rate)
			{
				phase = 0.0;
				phase_last = -1.0;
				sample_last = 0.0;
			}

			UnitRef input;
			UnitRef sample_rate;
			UnitRef bit_rate;

			float phase;
			float phase_last;
			float sample_last;

			virtual void next(int count);
	};
}
