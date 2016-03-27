#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class WaveShaper : public UnaryOpNode
	{
		public:
			WaveShaper(NodeRef input, Buffer *buffer) :
				UnaryOpNode(input), buffer(buffer)
			{
				this->name = "waveshaper";
			}

			Buffer *buffer;
			virtual void next(sample **out, int num_frames);
	};
}
