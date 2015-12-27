#pragma once

#include "../unit.h"
#include "../constants.h"

#include <Accelerate/Accelerate.h>

#include <vector>

namespace signum
{
	class IFFT : public UnaryOpUnit
	{
		public:
			IFFT(UnitRef input = 0.0, int fft_size = 512) :
				UnaryOpUnit(input), N(fft_size)
			{
				this->name = "ifft";

				this->log2N = (int) log2((float) N);
				this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

				this->buffer = (sample *) malloc(N * sizeof(sample));
			}

			~IFFT()
			{
				// free resources
			}

			int N;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;

			virtual void next(sample **out, int num_frames)
			{
				assert(num_frames == N);

				DSPSplitComplex buffer_split = { buffer, buffer + N/2 };
				DSPSplitComplex input_split = { this->input->out[0], this->input->out[0] + N/2 };

				vDSP_fft_zrop(fft_setup, &input_split, 1, &buffer_split, 1, log2N, FFT_INVERSE);
				vDSP_ztoc(&buffer_split, 1, (DSPComplex *) out[0], 2, N / 2);

				float scale = 1.0 / (N * 2.0);
				vDSP_vsmul(out[0], 1, &scale, out[0], 1, N);
			}
	};

	REGISTER(IFFT, "ifft");
}

