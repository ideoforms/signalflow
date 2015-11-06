#pragma once 

#include "../unit.h"

namespace signum::gen
{
	class SineWave : public Unit
	{
	public:
		SineWave(int frequency);

		int frequency;
		int phase;

		virtual void next(int count);
	};
}
