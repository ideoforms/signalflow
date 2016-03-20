#pragma once

#include "../unit.h"

namespace libsignal
{
	class Width : public Unit
	{
		public:
			Width(UnitRef input, UnitRef width) :
				input(input), width(width)
			{
				this->channels_in = 1;
				this->channels_out = 2;
			}

			UnitRef input;
			UnitRef width;

			virtual void next(sample **out, int num_frames);
	};
}
