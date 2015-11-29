#pragma once 

#include "../unit.h"

namespace signum::rnd
{
	class Dust : public Unit
	{
	public:
		Dust(UnitRef frequency = 1.0);

		UnitRef frequency = nullptr;

		virtual void next(sample **out, int num_frames);
	
	private:
		int steps_remaining;

	};

	REGISTER(Dust, "dust");
}
