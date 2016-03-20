#include "../unit.h"
#include "../constants.h"
#include "../buffer.h"

namespace libsignal
{
	class Sampler : public Unit
	{
		public:
			Sampler(Buffer *buffer, UnitRef rate = 1.0, bool loop = false);

			Buffer *buffer;

			UnitRef rate;
			float phase;
			bool loop;

			virtual void trigger();
			virtual void next(sample **out, int num_frames);
	};
}
