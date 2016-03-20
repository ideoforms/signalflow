#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"
#include "../util.h"

#include <list>

namespace libsignal
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
			float out_channel_pan,
				  in_channel_pan;

			this->zero_output();

			for (int out_channel = 0; out_channel < this->channels; out_channel++)
			{
				/*------------------------------------------------------------------------
				 * out_channel_pan ranges from [0..1], where
				 * leftmost channel = 0, rightmost channel = 1
				 *-----------------------------------------------------------------------*/
				if (this->channels > 1)
					out_channel_pan = map(out_channel, 0, this->channels - 1, 0, 1);
				else
					out_channel_pan = 0.5;

				for (int in_channel = 0; in_channel < this->channels_in; in_channel++)
				{
					float channel_amp = 1.0;
					if (this->channels > 1)
					{
						if (this->channels_in > 1)
							in_channel_pan = map(in_channel, 0, this->channels_in - 1, 0, 1);
						else
							in_channel_pan = 0.5;

						float channel_distance = fabs(in_channel_pan - out_channel_pan);
						float channel_distance_max = 1.0 / (this->channels - 1);
						channel_amp = map(channel_distance,
								channel_distance_max, 0,
								0, 1);
						channel_amp = clip(channel_amp, 0, 1);
					}
					
					for (int frame = 0; frame < num_frames; frame++)
					{
						out[out_channel][frame] += channel_amp * this->input->out[in_channel][frame];
					}
				}
			}
		}

		virtual void update_channels()
		{
			this->channels_in = this->min_input_channels = this->max_input_channels = this->input->channels_out;
			this->channels_out = this->min_output_channels = this->max_output_channels = this->channels;

			signal_debug("Unit %s set num_out_channels to %d", this->name.c_str(), this->channels_out);
		}

		std::list <UnitRef> inputs;
		int channels;

	};

	REGISTER(Mixer, "mixer");

}

