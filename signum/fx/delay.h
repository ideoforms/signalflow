#pragma once

#include "../unit.h"
#include "../constants.h"
#include "../ringbuffer.h"

namespace signum::fx
{
	class Delay : public Unit
	{
		public:
			Delay(UnitRef input, UnitRef delaytime, UnitRef feedback, float maxdelaytime = 10.0) :
				input(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime), buffer(maxdelaytime * 44100.0)
				{ }

			UnitRef input;
			UnitRef delaytime;
			UnitRef feedback;
			float maxdelaytime;

			RingBuffer <sample> buffer;

			virtual sample next();
	};
}
