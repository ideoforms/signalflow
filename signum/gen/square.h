#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class SquareWave : public Unit
	{
	public:
		SquareWave(int frequency, float width = 0.5);

		int frequency;
		float phase;
		float width;

		virtual void next(int count);
	};
}
