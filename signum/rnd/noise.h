#pragma once 

#include "../unit.h"

namespace signum::rnd
{
	class Noise : public Unit
	{
	public:
		Noise(UnitRef frequency, bool interpolate = false, float min = 0.0, float max = 1.0);

		UnitRef frequency;
		bool interpolate;
		int phase;
		sample value;

		float min;
		float max;

		virtual void next(sample **out, int num_frames);
	
	private:
		int steps_remaining;
		float step_change;

	};
}
