#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class SquareWave : public Unit
	{
	public:
		SquareWave(int frequency, float width);

		int frequency;
		float phase;
		float width;

		virtual void next(Buffer &buffer, int count);
	};
}
