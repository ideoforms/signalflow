#pragma once 

#include "../node.h"

namespace libsignal
{
	class Sine : public Node
	{
	public:
		Sine(NodeRef frequency = 440) : frequency(frequency)
		{
			this->phase = 0;
			this->name = "sine";
			this->add_param("frequency", this->frequency);
		}

		NodeRef frequency;
		float phase;

		virtual void next(sample **out, int num_frames);
	};

	REGISTER(Sine, "sine");

}

