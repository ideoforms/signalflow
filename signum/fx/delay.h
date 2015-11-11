#pragma once

#include "../unit.h"
#include "../constants.h"
#include "../ringbuffer.h"

namespace signum::fx
{
	class Delay : public Unit
	{
		public:
			Delay(Unit *input, float delaytime, float feedback = 0.0, float maxdelaytime = 10.0) :
				input(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime), buffer(maxdelaytime * 44100.0)
				{ }

			Unit *input;
			float delaytime;
			float feedback;
			float maxdelaytime;

			RingBuffer <sample> buffer;

			virtual void next(int count);
	};
}
