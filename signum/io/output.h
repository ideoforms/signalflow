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
		int add_input(Unit &unit);
		void next(int count);

		struct SoundIo *soundio;
		struct SoundIoDevice *device;
		struct SoundIoOutStream *outstream;

		std::vector <Unit *> inputs;
		std::vector <Buffer *> input_buffers;
	};
}
