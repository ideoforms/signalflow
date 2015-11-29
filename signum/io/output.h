#pragma once

#include <soundio/soundio.h>

#include "../unit.h"
#include "../buffer.h"
#include "../graph.h"

#include <vector>

namespace signum::io
{
	class AudioOut : public Unit
	{
	public:
		AudioOut(Graph *graph);
		int init();
		int start();
		int close();
		virtual void next(sample **out, int num_samples);

		struct SoundIo *soundio;
		struct SoundIoDevice *device;
		struct SoundIoOutStream *outstream;

		virtual void add_input(const UnitRef &input)
		{
			inputs.push_back(input);
		}
	};
}
