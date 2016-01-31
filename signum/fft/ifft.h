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
			IFFT(UnitRef input = 0.0, int fft_size = 1024) :
				UnaryOpUnit(input), N(fft_size)
			{
				this->name = "ifft";

				this->log2N = (int) log2((float) N);
				this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

				this->buffer = (sample *) malloc(N * sizeof(sample));
				this->buffer2 = (sample *) malloc(N * sizeof(sample));
			}

			~IFFT()
			{
				// free resources
			}

			int N;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;
			sample *buffer2;

			virtual void next(sample **out, int num_frames)
			{
				assert(num_frames == N);

				/*------------------------------------------------------------------------
				 * Set up pointers to memory spaces so we can treat input and buffer
				 * as split-valued complex pairs.
				 *-----------------------------------------------------------------------*/
				DSPSplitComplex buffer_split = { buffer, buffer + N/2 };
				DSPSplitComplex input_split = { this->input->out[0], this->input->out[0] + N/2 };

				/*------------------------------------------------------------------------
				 * Convert magnitude/phase to complex values.
				 * Received values are split but vDSP_rect requires that pairs be
				 * given sequentially, thus do a small dance.
				 *-----------------------------------------------------------------------*/
				vDSP_ztoc(&input_split, 1, (DSPComplex *) this->buffer, 2, N / 2);
				vDSP_rect(this->buffer, 2, this->buffer2, 2, N / 2);

				/*------------------------------------------------------------------------
				 * 1. Expecting polar values
				 *-----------------------------------------------------------------------*/
				vDSP_ctoz((DSPComplex *) this->buffer2, 2, &buffer_split, 1, N / 2);

				/*------------------------------------------------------------------------
				 * 1. Expecting Cartesian values
				 *-----------------------------------------------------------------------*/
				// vDSP_ctoz((DSPComplex *) this->input->out[0], 2, &buffer_split, 1, N / 2);

				/*------------------------------------------------------------------------
				 * Perform inverse FFT
				 *-----------------------------------------------------------------------*/
				vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_INVERSE);
				vDSP_ztoc(&buffer_split, 1, (DSPComplex *) out[0], 2, N / 2);

				/*------------------------------------------------------------------------
				 * Scale down (Required by vDSP)
				 *-----------------------------------------------------------------------*/
				float scale = 1.0 / (N * 2.0);
				vDSP_vsmul(out[0], 1, &scale, out[0], 1, N);
			}
	};

	REGISTER(IFFT, "ifft");
}

