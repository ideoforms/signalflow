#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class Square : public Unit
	{
	public:
		Square(float frequency, float width = 0.5);

		float frequency;
		float phase;
		float width;

		virtual void next(int count);
	};
}
