#include "../unit.h"
#include "../constants.h"

namespace signum::gen
{
	class Sampler : public Unit
	{
		public:
			Sampler(const char *filename, float rate = 1.0, bool loop = false);
			Sampler(Buffer &buffer, float rate = 1.0, bool loop = false);

			Buffer *buffer;

			Unit *rate;
			float phase;
			bool loop;

			virtual void trigger();
			virtual void next(int count);
	};
}
