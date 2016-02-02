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
				this->magnitudes = this->out[0];
				this->phases = this->out[0] + this->num_bins;
			}

			sample *magnitudes;
			sample *phases;
			int fft_size;
			int num_bins;
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

