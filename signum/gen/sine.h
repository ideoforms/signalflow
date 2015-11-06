#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class Sine : public Unit
	{
	public:
		Sine(float frequency);

		float frequency;
		int phase;

		virtual void next(int count);
	};
}
