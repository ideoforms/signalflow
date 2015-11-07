#include "../unit.h"
#include "../constants.h"

#include <vector>

namespace signum::gen
{
	class Grain
	{
		public:
			Grain(Buffer &buffer, int start, int length) : buffer(&buffer), sample_start(start), sample_length(length)
			{
				this->samples_done = 0;
			}

			sample next()
			{
				if (samples_done < sample_length)
					return 
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
			Granulator(Buffer &buffer, Unit &clock, Unit &pos, float grain_length = 0.1);

			Buffer *buffer;
			Unit *pos;
			Unit *clock;

			float grain_length;

			virtual void next(int count);

		private:
			sample clock_last;
			int grain_buffer_sample_start;
			int grain_samples_done;
			int grain_samples_total;

			std::vector <Grain> grains;
	};

}
