#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"

#include <list>

namespace signum
{

	class Mixer : public UnaryOpUnit
	{

	public:

		Mixer(UnitRef input = 0, int channels = 1) : UnaryOpUnit(input), channels(channels)
		{
			this->name = "mixer";
			this->channels = channels;
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int out_channel = 0; out_channel < this->channels; out_channel++)
			{
				for (int frame = 0; frame < num_frames; frame++)
				{
					out[out_channel][frame] = 0.0;
					for (int channel = 0; channel < this->channels_in; channel++)
					{
						out[out_channel][frame] += this->input->out[channel][frame];
					}
				}
			}
		}

		virtual void update_channels()
		{
			this->channels_in = this->min_input_channels = this->max_input_channels = this->input->channels_out;
			this->channels_out = this->min_output_channels = this->max_output_channels = this->channels;

			signum_debug("Unit %s set num_out_channels to %d", this->name.c_str(), this->channels_out);
		}

		std::list <UnitRef> inputs;
		int channels;

	};

	REGISTER(Mixer, "mixer");

}

