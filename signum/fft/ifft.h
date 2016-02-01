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
				UnaryOpUnit(input), fft_size(fft_size)
			{
				this->name = "ifft";

				this->log2N = (int) log2((float) fft_size);
				this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

				/*------------------------------------------------------------------------
				 * Buffers used in intermediate FFT calculations.
				 *-----------------------------------------------------------------------*/
				this->buffer = (sample *) malloc(fft_size * sizeof(sample));
				this->buffer2 = (sample *) malloc(fft_size * sizeof(sample));

				this->hop_size = fft_size / 4;

				/*------------------------------------------------------------------------
				 * Generate a Hann window for overlap-add.
				 *-----------------------------------------------------------------------*/
                this->window = (sample *) calloc(fft_size, sizeof(sample));
                vDSP_hann_window(this->window, fft_size, vDSP_HANN_NORM);
			}

			~IFFT()
			{
				// free resources
			}

			int fft_size;
			int hop_size;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;
			sample *buffer2;
			sample *window;

			virtual void ifft(sample *in, sample *out, bool polar = true, bool do_window = true)
			{
				/*------------------------------------------------------------------------
				 * Set up pointers to memory spaces so we can treat input and buffer
				 * as split-valued complex pairs.
				 *-----------------------------------------------------------------------*/
				DSPSplitComplex buffer_split = { buffer, buffer + fft_size/2 };
				DSPSplitComplex input_split = { in, in + fft_size/2 };

				/*------------------------------------------------------------------------
				 * Convert magnitude/phase to complex values.
				 * Received values are split but vDSP_rect requires that pairs be
				 * given sequentially, thus do a small dance.
				 *-----------------------------------------------------------------------*/
				vDSP_ztoc(&input_split, 1, (DSPComplex *) this->buffer, 2, fft_size / 2);
				vDSP_rect(this->buffer, 2, this->buffer2, 2, fft_size / 2);

				/*------------------------------------------------------------------------
				 * 1. Expecting polar values
				 *-----------------------------------------------------------------------*/
				if (polar)
				{
					vDSP_ctoz((DSPComplex *) this->buffer2, 2, &buffer_split, 1, fft_size / 2);
				}

				/*------------------------------------------------------------------------
				 * 2. Expecting Cartesian values
				 *-----------------------------------------------------------------------*/
				else
				{
					vDSP_ctoz((DSPComplex *) this->input->out[0], 2, &buffer_split, 1, fft_size / 2);
				}

				/*------------------------------------------------------------------------
				 * Perform inverse FFT
				 *-----------------------------------------------------------------------*/
				vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_INVERSE);
				vDSP_ztoc(&buffer_split, 1, (DSPComplex *) this->buffer2, 2, fft_size / 2);

				/*------------------------------------------------------------------------
				 * Scale down (Required by vDSP)
				 *-----------------------------------------------------------------------*/
				float scale = 1.0 / (fft_size * 2.0);
				vDSP_vsmul(buffer2, 1, &scale, buffer2, 1, fft_size);

				/*------------------------------------------------------------------------
				 * Apply Hann window (for overlap-add)
				 *-----------------------------------------------------------------------*/
				if (do_window)
					vDSP_vmul(buffer2, 1, this->window, 1, buffer2, 1, fft_size);

				/*------------------------------------------------------------------------
				 * Add to output buffer (for overlap/add)
				 *-----------------------------------------------------------------------*/
				vDSP_vadd(buffer2, 1, out, 1, out, 1, fft_size);
			}

			virtual void next(sample **out, int num_frames)
			{
				assert(num_frames == fft_size);

				/*------------------------------------------------------------------------
				 * 
				 *-----------------------------------------------------------------------*/
				memcpy(out[0], out[0] + this->fft_size, this->fft_size * sizeof(sample));
				memset(out[0] + this->fft_size, 0, this->fft_size * sizeof(sample));

				int num_hops = this->fft_size / this->hop_size;

				/*------------------------------------------------------------------------
				 * Perform repeated inverse FFT, moving forward hop_size frames per
				 * hop.
				 *-----------------------------------------------------------------------*/
				for (int hop = 0; hop < num_hops; hop++)
				{
					this->ifft(this->input->out[hop], out[0] + (hop * hop_size));
				}

			}
	};

	REGISTER(IFFT, "ifft");
}

