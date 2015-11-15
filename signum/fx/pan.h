#pragma once

#include "../unit.h"

namespace signum::fx
{
	class Pan : public Unit
	{
		public:
			Pan(int channels, UnitRef input, UnitRef pan) :
				input(input), pan(pan)
			{
				this->channels_in = 1;
				this->channels_out = channels;
			}

			UnitRef input;
			UnitRef pan;

			virtual void next(sample *in, sample *out);
	};
}
