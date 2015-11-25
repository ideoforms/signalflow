#pragma once

#include "../unit.h"
#include "../constants.h"
#include "../ringbuffer.h"

#include <vector>

namespace signum::fx
{
	class Delay : public Unit
	{
		public:
			Delay(UnitRef input, UnitRef delaytime, UnitRef feedback, float maxdelaytime = 10.0) :
				input(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime)
			{
				for (int i = 0; i < input->channels_out; i++)
					buffers.push_back(SampleRingBuffer(maxdelaytime * 44100.0));
			}

			UnitRef input;
			UnitRef delaytime;
			UnitRef feedback;
			float maxdelaytime;

			std::vector <SampleRingBuffer> buffers;

			virtual void next(sample **out, int num_frames);
	};
}
