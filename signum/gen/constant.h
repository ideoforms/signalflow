#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class Constant : public Unit
	{
	public:
		Constant(sample value);

		float value;

		virtual sample next();
	};
}
