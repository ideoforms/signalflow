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

		// TODO: Should we assert a buffer is mono before indexing?
		sample get(int index) { return this->data[0][index]; }
		sample operator [](int index){ return this->get(index); }

		int num_channels;
		int num_frames;
		sample **data;

		float duration;
	};
}
