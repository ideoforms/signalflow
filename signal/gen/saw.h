#pragma once 

#include "../node.h"

namespace libsignal
{
	class Saw : public Node
	{
	public:
		Saw(NodeRef frequency) : frequency(frequency), phase(0)
		{
			this->add_param("frequency", this->frequency);
		};

		NodeRef frequency;

		float phase;

		virtual void next(sample **out, int num_frames);
	};
}
