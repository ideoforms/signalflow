#pragma once 

#include "../unit.h"

namespace signum
{
	class Noise : public Unit
	{
	public:
		Noise(UnitRef frequency = 0.0, bool interpolate = true, UnitRef min = 0.0, UnitRef max = 1.0);

		UnitRef frequency;
		bool interpolate;
		int phase;
		sample value;

		UnitRef min;
		UnitRef max;

		virtual void next(sample **out, int num_frames);
	
	private:
		int steps_remaining;
		float step_change;

	};

	REGISTER(Noise, "noise");
}
