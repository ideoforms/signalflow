#pragma once 

#include "../unit.h"

namespace signum::rnd
{
	class Dust : public Unit
	{
	public:
		Dust(float frequency);

		float frequency;

		virtual void next(int count);
	
	private:
		int steps_remaining;

	};
}
