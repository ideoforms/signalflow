#pragma once

#include "../unit.h"
#include "../constants.h"

namespace signum
{
	class FFTLPF : public UnaryOpUnit
	{
		public:
			FFTLPF(UnitRef input = 0, UnitRef frequency = 0.5) :
				UnaryOpUnit(input), frequency(frequency)
			{
				this->name = "fft_lpf";
			}

			UnitRef frequency;

			virtual void next(sample **out, int num_frames)
			{
				int fft_size = num_frames;
				int num_bins = num_frames / 2;
				int cutoff = this->frequency->out[0][0];

				/*------------------------------------------------------------------------
				 * Calculate a normalised cutoff value [0, 1]
				 *-----------------------------------------------------------------------*/
				float cutoff_norm = cutoff / (this->graph->sample_rate / 2.0);

				/*------------------------------------------------------------------------
				 * Calculate the bin above which we want to set magnitude = 0
				 *-----------------------------------------------------------------------*/
				int cutoff_bin = num_bins * cutoff_norm;
	
				for (int i = 0; i < num_frames; i++)
				{
					if (i < num_frames / 2)
					{
						if (i > num_frames / 80)
							out[0][i] = 0.0;
						else
							out[0][i] = input->out[0][i];
					}
					else
						out[0][i] = input->out[0][i];
				}
			}
	};

	REGISTER(FFTLPF, "fft_lpf");
}
