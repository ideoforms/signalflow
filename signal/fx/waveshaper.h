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
				memset(sample_last, 0, sizeof(sample_last));

				this->name = "waveshaper";
			}

			virtual void next(sample **out, int num_frames);
	};
}
