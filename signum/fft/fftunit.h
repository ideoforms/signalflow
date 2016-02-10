#pragma once

#include "../unit.h"

namespace signum
{
	class FFTUnit : public Unit
	{
		public:

			FFTUnit(int fft_size) : Unit()
			{
				this->fft_size = fft_size;
				this->num_bins = fft_size / 2;
				this->num_hops = 0;

				this->magnitudes = (sample **) malloc(SIGNUM_MAX_CHANNELS * sizeof(float *));
				for (int i = 0; i < SIGNUM_MAX_CHANNELS; i++)
					this->magnitudes[i] = this->out[i];

				this->phases = (sample **) malloc(SIGNUM_MAX_CHANNELS * sizeof(float *));
				for (int i = 0; i < SIGNUM_MAX_CHANNELS; i++)
					this->phases[i] = this->out[i] + this->num_bins;
			}

			~FFTUnit()
			{
				free(this->magnitudes);
				free(this->phases);
			}

			sample **magnitudes;
			sample **phases;
			int fft_size;
			int num_bins;
			int num_hops;
	};

	class FFTOpUnit : public FFTUnit
	{
		public:

			FFTOpUnit(UnitRef input) : FFTUnit(((FFTUnit *) input.get())->fft_size), input(input)
			{
				this->add_param("input", this->input);
			}

			UnitRef input;
	};
}

