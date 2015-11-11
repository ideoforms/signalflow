#include "../unit.h"
#include "../constants.h"

#include <vector>

namespace signum::gen
{
	class Grain
	{
		public:
			Grain(Buffer *buffer, int start, int length) : buffer(buffer), sample_start(start), sample_length(length)
			{
				this->samples_done = 0;
			}

			bool finished()
			{
				return this->samples_done >= this->sample_length;
			}

			Buffer *buffer;
			int sample_start;
			int sample_length;
			int samples_done;
	};

	class Granulator : public Unit
	{
		public:
			Granulator(Buffer *buffer, UnitRef clock, UnitRef pos, float grain_length = 0.1);

			Buffer *buffer;
			UnitRef pos;
			UnitRef clock;

			float grain_length;

			virtual void next(int count);

		private:
			sample clock_last;

			std::vector <Grain *> grains;
	};

}
