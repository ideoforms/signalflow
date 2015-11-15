#pragma once

#include "../unit.h"

namespace signum::fx
{
	class Width : public Unit
	{
		public:
			Width(UnitRef input, UnitRef width) :
				input(input), width(width)
			{
				this->num_input_channels = 2;
				this->num_output_channels = 2;
			}

			UnitRef input;
			UnitRef width;

			virtual void next(sample *in, sample *out);
	};
}
