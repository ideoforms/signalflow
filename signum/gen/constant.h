#pragma once 

#include "../unit.h"

namespace signum
{
	class Constant : public Unit
	{
	public:
		Constant(sample value = 0);

		float value;

		virtual void next(sample **out, int num_frames);
	};

	REGISTER(Constant, "constant");
}
