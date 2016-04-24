#pragma once 

#include "../node.h"

namespace libsignal
{
	class Sine : public Node
	{
	public:
		Sine(NodeRef frequency = 440) : frequency(frequency)
		{
			this->name = "sine";
			this->add_input("frequency", this->frequency);
			memset(this->phase, 0, sizeof(this->phase));
		}

		NodeRef frequency;
		float phase[SIGNAL_MAX_CHANNELS];

		virtual void process(sample **out, int num_frames);
	};

	REGISTER(Sine, "sine");
}

