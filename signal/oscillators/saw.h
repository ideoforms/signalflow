#pragma once 

#include "../node.h"

namespace libsignal
{
	class Saw : public Node
	{
	public:
		Saw(NodeRef frequency = 440) : frequency(frequency)
		{
			this->name = "saw";
			this->add_input("frequency", this->frequency);
			memset(this->phase, 0, sizeof(this->phase));
		};

		NodeRef frequency;

		float phase[SIGNAL_MAX_CHANNELS];

		virtual void process(sample **out, int num_frames);
	};

	REGISTER(Saw, "saw");
}
