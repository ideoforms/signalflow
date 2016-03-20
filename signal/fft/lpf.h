#pragma once

#include "fftunit.h"

namespace libsignal
{
	class FFTLPF : public UnaryOpUnit
	{
		public:
			FFTLPF(UnitRef input = 0, UnitRef frequency = 2000) :
				UnaryOpUnit(input), frequency(frequency)
			{
				this->name = "fft_lpf";
				this->add_param("frequency", this->frequency);
			}

			UnitRef frequency;

			virtual void next(sample **out, int num_frames)
			{
				for (int channel = 0; channel < 4; channel++)
				{
					int fft_size = num_frames;
					int num_bins = num_frames / 2;
					float cutoff = this->frequency->out[0][0];

					/*------------------------------------------------------------------------
					 * Calculate a normalised cutoff value [0, 1]
					 *-----------------------------------------------------------------------*/
					float cutoff_norm = (float) cutoff / (this->graph->sample_rate / 2.0);

					/*------------------------------------------------------------------------
					 * Calculate the bin above which we want to set magnitude = 0
					 *-----------------------------------------------------------------------*/
					int cutoff_bin = num_bins * cutoff_norm;
		
					for (int i = 0; i < num_frames; i++)
					{
						if (i < num_frames / 2)
						{
							if (i > cutoff_bin)
								out[channel][i] = 0.0;
							else
								out[channel][i] = input->out[channel][i];
						}
						else
							out[channel][i] = input->out[channel][i];
					}
				}
			}
		};

		REGISTER(FFTLPF, "fft_lpf");
}
