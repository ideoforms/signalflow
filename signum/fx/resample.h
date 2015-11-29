#pragma once

#include "../unit.h"
#include "../constants.h"

namespace signum::fx
{
	class Resample : public UnaryOpUnit
	{
		public:
			Resample(UnitRef input, UnitRef sample_rate, UnitRef bit_rate) :
				UnaryOpUnit(input), sample_rate(sample_rate), bit_rate(bit_rate)
			{
				phase = 0.0;
				phase_last = -1.0;
				memset(sample_last, 0, sizeof(sample_last));

				this->name = "resample";
				this->add_param("sample_rate", this->sample_rate);
				this->add_param("bit_rate", this->bit_rate);
			}

			UnitRef sample_rate;
			UnitRef bit_rate;

			float phase;
			float phase_last;
			sample sample_last[32];

			virtual void next(sample **out, int num_frames);
	};
}
