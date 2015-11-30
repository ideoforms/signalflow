#pragma once

#include "../unit.h"

namespace signum::fx
{
	class Pan : public Unit
	{
		public:
			Pan(int channels = 2, UnitRef input = 2, UnitRef pan = 0.5) :
				input(input), pan(pan)
			{
				this->channels_in = 1;
				this->channels_out = channels;

				this->add_param("input", this->input);
				this->add_param("pan", this->pan);

				this->name = "pan";
			}

			UnitRef input;
			UnitRef pan;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Pan, "pan");
}
