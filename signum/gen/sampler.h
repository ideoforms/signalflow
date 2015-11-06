#include "../unit.h"
#include "../constants.h"

namespace signum::gen
{
	class Sampler : public Unit
	{
		public:
			Sampler(const char *filename, float rate = 1.0, bool loop = false);

			float *buffer;
			long long num_frames;

			float rate;
			float phase;
			bool loop;

			virtual void trigger();
			virtual void next(int count);
	};
}
