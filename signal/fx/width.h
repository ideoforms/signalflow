#pragma once

#include "../node.h"

namespace libsignal
{
	class Width : public Node
	{
		public:
			Width(NodeRef input, NodeRef width) :
				input(input), width(width)
			{
				this->channels_in = 1;
				this->channels_out = 2;
			}

			NodeRef input;
			NodeRef width;

			virtual void next(sample **out, int num_frames);
	};
}