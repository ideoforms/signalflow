#pragma once 

#include "../node.h"

namespace libsignal
{
	class Saw : public Node
	{
	public:
		Saw(NodeRef frequency) : frequency(frequency)
		{
			this->add_param("frequency", this->frequency);
			memset(this->phase, 0, sizeof(this->phase));
		};

		NodeRef frequency;

		float phase[SIGNAL_MAX_CHANNELS];

		virtual void next(sample **out, int num_frames);
	};
}
