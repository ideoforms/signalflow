#pragma once 

#include "../unit.h"

namespace signum::rnd
{
	class Noise : public Unit
	{
	public:
		Noise(float frequency = 0, bool interpolate = false, float min = 0.0, float max = 1.0);

		float frequency;
		bool interpolate;
		int phase;
		sample value;

		float min;
		float max;

		virtual sample next();
	
	private:
		int steps_remaining;
		float step_change;

	};
}
