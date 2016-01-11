#pragma once

#include <soundio/soundio.h>

#include "../unit.h"
#include "../buffer.h"
#include "../graph.h"

#include <vector>

namespace signum
{
	class AudioIn : public Unit
	{
	public:
		AudioIn();
		int init();
		int start();
		int close();
		virtual void next(sample **out, int num_samples);

		struct SoundIo *soundio;
		struct SoundIoDevice *device;
		struct SoundIoInStream *instream;

		Buffer *buffer;
		int read_pos;
		int write_pos;
	};
}
