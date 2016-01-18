#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"

namespace signum
{

	class Multiplex : public Unit
	{

	public:

		Multiplex() : Unit()
		{
			this->name = "multiplex";
		}

		virtual void next(sample **out, int num_frames)
		{
			int channel = 0; 
			for (UnitRef input : this->inputs)
			{
				for (int channel = 0; channel < input->channels_out; channel++)
				{
					memcpy(out[channel], input->out[channel], num_frames * sizeof(sample));
				}
				channel += input->channels_out;
			}
		}

		virtual void update_channels()
		{
			this->channels_in = 0;
			for (UnitRef input : this->inputs)
			{
				this->channels_in += input->channels_out;
			}

			this->channels_out = this->channels_in;
			// signum_debug("Unit %s set num_out_channels to %d", this->name.c_str(), this->channels_out);
		}

		virtual void add_input(UnitRef input)
		{
			this->inputs.push_back(input);
		}

		std::vector <UnitRef> inputs;
	};

	REGISTER(Multiplex, "multiplex");

}

