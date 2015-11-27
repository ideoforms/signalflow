#include "../unit.h"
#include "../constants.h"
#include "../buffer.h"

#include <vector>

namespace signum::gen
{
	class Grain
	{
		public:
			Grain(Buffer *buffer, int start, int length, float pan) :
				buffer(buffer), sample_start(start), sample_length(length), pan(pan)
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
			float pan;
	};

	class Granulator : public Unit
	{
		public:
			Granulator(Buffer *buffer, UnitRef clock, UnitRef pos, UnitRef grain_length = 0.1);

			Buffer *buffer;
			EnvelopeBuffer *envelope;

			UnitRef pos;
			UnitRef clock;
			UnitRef grain_length;
			UnitRef pan;

			virtual void next(sample **out, int num_frames);
			virtual void set_spatialisation(int num_channels, UnitRef pan);

		private:
			sample clock_last;

			std::vector <Grain *> grains;
	};

}
