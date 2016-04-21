#pragma once

#include "../node.h"
#include "../constants.h"
#include "../ringbuffer.h"
#include "../graph.h"

#include <vector>

namespace libsignal
{
	class Delay : public UnaryOpNode
	{
		public:
			Delay(NodeRef input = 0.0, NodeRef delaytime = 0.1, NodeRef feedback = 0.5, float maxdelaytime = 10.0) :
				UnaryOpNode(input), delaytime(delaytime), feedback(feedback), maxdelaytime(maxdelaytime)
			{
				this->name = "delay";
				this->add_param("delay_time", this->delaytime);
				this->add_param("feedback", this->feedback);

				for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
					buffers.push_back(new SampleRingBuffer(maxdelaytime * this->graph->sample_rate));
			}

			~Delay()
			{
				// free buffers
			}

			NodeRef delaytime;
			NodeRef feedback;
			float maxdelaytime;

			std::vector <SampleRingBuffer *> buffers;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Delay, "delay");
}
