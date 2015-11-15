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
				this->num_input_channels = 1;
				this->num_output_channels = channels;
			}

			UnitRef input;
			UnitRef pan;

			virtual void next(sample *in, sample *out);
	};
}
