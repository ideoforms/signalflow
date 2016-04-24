#pragma once 

#include "../node.h"

namespace libsignal
{
	class Square : public Node
	{
	public:
		Square(NodeRef frequency = 440, NodeRef width = 0.5) : frequency(frequency), width(width)
		{
			this->name = "square";
			this->add_param("frequency", this->frequency);
			this->add_param("width", this->width);
			memset(this->phase, 0, sizeof(this->phase));
		};

		NodeRef frequency;
		NodeRef width;

		float phase[SIGNAL_MAX_CHANNELS];

		virtual void process(sample **out, int num_frames);
	};

	REGISTER(Square, "square");
}
