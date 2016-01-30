#pragma once

#include "../unit.h"
#include "../constants.h"
#include "../ringbuffer.h"

#include <vector>

namespace signum
{
	class Delay : public UnaryOpUnit
	{
		public:
			Delay(UnitRef input = 0.0, UnitRef delaytime = 0.1, UnitRef feedback = 0.5, float maxdelaytime = 10.0) :
				UnaryOpUnit(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime)
			{
				this->name = "delay";
				this->add_param("delay_time", this->delaytime);
				this->add_param("feedback", this->feedback);

				for (int i = 0; i < SIGNUM_MAX_CHANNELS; i++)
					buffers.push_back(new SampleRingBuffer(maxdelaytime * 44100.0));
			}

			~Delay()
			{
				// free buffers
			}

			UnitRef delaytime;
			UnitRef feedback;
			float maxdelaytime;

			std::vector <SampleRingBuffer *> buffers;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Delay, "delay");
}
