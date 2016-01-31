#pragma once

#include "../unit.h"
#include "../constants.h"

#include <Accelerate/Accelerate.h>
#include <assert.h>

#include <vector>

namespace signum
{
	class FFT : public UnaryOpUnit
	{
		public:
			FFT(UnitRef input = 0.0, int fft_size = 1024) :
				UnaryOpUnit(input), fft_size(fft_size)
			{
				this->name = "fft";

				this->log2N = (int) log2((float) fft_size);
				this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

				this->buffer = (sample *) calloc(fft_size, sizeof(sample));
				this->buffer2 = (sample *) calloc(fft_size, sizeof(sample));

				this->hop_size = fft_size / 2.0;
				this->inbuf = (sample *) calloc(fft_size * 2, sizeof(sample));
				this->inbuf_size = 0;
				this->window = (sample *) calloc(fft_size, sizeof(sample));
				vDSP_hann_window(this->window, fft_size, vDSP_HANN_NORM);
			}

			~FFT()
			{
				vDSP_destroy_fftsetup(this->fft_setup);
				free(this->buffer);
			}

			int fft_size;
			int hop_size;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;
			sample *buffer2;
			sample *inbuf;
			int inbuf_size;
			sample *window;

			virtual void fft(sample *in, sample *out, bool polar = true)
			{
				DSPSplitComplex buffer_split = { buffer, buffer + fft_size/2 };
				DSPSplitComplex output_split = { out, out + fft_size/2 };

				vDSP_vmul(in, 1, this->window, 1, buffer2, 1, fft_size);

				/*------------------------------------------------------------------------
				 * Convert from interleaved format (sequential pairs) to split format,
				 * as required by the vDSP real-to-complex functions.
				 * 
				 * Stride of 2 is the smallest permissible value, denoting no gap
				 * between "pairs" of samples".
				 *-----------------------------------------------------------------------*/
				vDSP_ctoz((DSPComplex *) buffer2, 2, &buffer_split, 1, fft_size/2);

				/*------------------------------------------------------------------------
				 * Perform single-precision FFT, in-place (overwriting real values).
				 *-----------------------------------------------------------------------*/
				vDSP_fft_zrip(fft_setup, &buffer_split, 1, log2N, FFT_FORWARD);

				/*------------------------------------------------------------------------
				 * Now, calculate magnitudes and phases, stored in our output buffer.
				 *-----------------------------------------------------------------------*/

				/*------------------------------------------------------------------------
				 * 2. Sending polar values
				 *-----------------------------------------------------------------------*/
				if (polar)
				{
					vDSP_ztoc(&buffer_split, 1, (DSPComplex *) buffer2, 2, fft_size/2);
					vDSP_polar(buffer2, 2, buffer, 2, fft_size/2);
					vDSP_ctoz((DSPComplex *) buffer, 2, &output_split, 1, fft_size/2);
				}

				/*------------------------------------------------------------------------
				 * 1. Sending cartesian values
				 *-----------------------------------------------------------------------*/
				else
				{
					vDSP_ztoc(&buffer_split, 1, (DSPComplex *) out, 2, fft_size/2);
				}

			}

			virtual void next(sample **out, int num_frames)
			{
				/*------------------------------------------------------------------------
				 * 
				 *-----------------------------------------------------------------------*/
				// assert(this->inbuf_size + num_frames < this->fft_size * 2);
				// memcpy(this->inbuf[this->inbuf_size], this->input->out[0], num_frames)
				// this->inbuf_size += num_frames;

				this->fft(this->input->out[0], this->out[0]);
			}
	};

	REGISTER(FFT, "fft");
}

