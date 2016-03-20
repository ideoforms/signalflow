#pragma once 

#include "../node.h"

namespace libsignal
{
	class Square : public Node
	{
	public:
		Square(NodeRef frequency, NodeRef width = 0.5) : frequency(frequency), width(width), phase(0)
		{
			this->add_param("frequency", this->frequency);
			this->add_param("width", this->width);
		};

		NodeRef frequency;
		NodeRef width;

		float phase;

		virtual void next(sample **out, int num_frames);
	};
}
