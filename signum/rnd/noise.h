#pragma once 

#include "../unit.h"

namespace signum::rnd
{
	class Noise : public Unit
	{
	public:
		Noise(float frequency = 0, bool interpolate = false);

		float frequency;
		bool interpolate;
		int phase;
		sample value;

		virtual void next(int count);
	
	private:
		int steps_remaining;
		float step_change;

	};
}
