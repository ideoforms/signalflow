#pragma once 

#include "../unit.h"

namespace libsignal
{
	class Square : public Unit
	{
	public:
		Square(UnitRef frequency, UnitRef width = 0.5) : frequency(frequency), width(width), phase(0)
		{
			this->add_param("frequency", this->frequency);
			this->add_param("width", this->width);
		};

		UnitRef frequency;
		UnitRef width;

		float phase;

		virtual void next(sample **out, int num_frames);
	};
}
