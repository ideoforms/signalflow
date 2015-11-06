#include "../unit.h"
#include "../constants.h"

namespace signum::gen
{
	class Sampler : public Unit
	{
		public:
			Sampler(const char *filename);

			float *buffer;
			long long num_frames;
			int phase;

			virtual void trigger();
			virtual void next(int count);
	};
}
