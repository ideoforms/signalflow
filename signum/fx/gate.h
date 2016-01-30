#pragma once

#include "../unit.h"
#include "../constants.h"

namespace signum
{
	class Gate : public UnaryOpUnit
	{
		public:
			Gate(UnitRef input = 0.0, UnitRef threshold = 0.1, UnitRef attack = 0.0, UnitRef hold = 0.0, UnitRef release = 0.0) :
				UnaryOpUnit(input), threshold(threshold), attack(attack), hold(hold), release(release)
			{
				this->open = false;
				this->openTime = 0;
				this->level = 0.0;

				this->name = "gate";
				this->add_param("threshold", this->threshold);
				this->add_param("attack", this->attack);
				this->add_param("hold", this->hold);
				this->add_param("release", this->release);
			}

			UnitRef sample_rate;
			UnitRef bit_rate;

			bool open;
			float openTime;
			float level;

			UnitRef threshold;

			// TODO: Implement attack, hold, release (based on current graph sample rate)
			UnitRef attack;
			UnitRef hold;
			UnitRef release;

			virtual void next(sample **out, int num_frames)
			{
				for (int frame = 0; frame < num_frames; frame++)
				{
					float amp = 0.0;
					for (int channel = 0; channel < this->channels_in; channel++)
						amp += this->input->out[channel][frame];
					amp /= this->channels_in;

					float threshold = this->threshold->out[0][frame];

					if (amp > threshold)
					{
						open = true;
						level = 1.0;
					}
					else
					{
						open = false;
						level = 0.0;
					}

					for (int channel = 0; channel < this->channels_out; channel++)
					{
						this->out[channel][frame] = this->input->out[channel][frame] * level;
					}
				}
			}
	};

	REGISTER(Gate, "gate");
}
