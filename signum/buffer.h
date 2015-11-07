#pragma once 

#include "constants.h"

namespace signum
{
	class Buffer
	{
	public:
		Buffer(int num_channels, int num_frames);

		Buffer(const char *filename);

		void open(const char *filename);

		int num_channels;
		int num_frames;
		sample **data;

		float duration;
	};
}
