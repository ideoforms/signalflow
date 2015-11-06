#include <soundio/soundio.h>

#include "../unit.h"
#include "../buffer.h"

#include <vector>

namespace signum::io
{
	class AudioOut : public Unit
	{
	public:
		AudioOut();
		int init();
		int start();
		int close();
		void next(int count);

		struct SoundIo *soundio;
		struct SoundIoDevice *device;
		struct SoundIoOutStream *outstream;

		std::vector <Buffer *> input_buffers;
	};
}
