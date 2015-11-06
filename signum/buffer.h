#pragma once 

#include "constants.h"

namespace signum
{
	class Buffer
	{
	public:
		Buffer(int num_channels, int num_frames);

		int num_channels;
		int num_frames;
		sample **data;
	};
}
