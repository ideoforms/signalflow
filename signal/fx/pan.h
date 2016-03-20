#pragma once

#include "../unit.h"

namespace libsignal
{
	class Pan : public Unit
	{
		public:
			Pan(int channels = 2, UnitRef input = 2, UnitRef pan = 0.5) :
				input(input), pan(pan)
			{
				this->channels_in = 1;
				this->channels_out = channels;

				this->min_input_channels = this->max_input_channels = 1;
				this->min_output_channels = this->max_output_channels = channels;

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
