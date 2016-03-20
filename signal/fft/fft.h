#pragma once

#include "fftnode.h"

#include <Accelerate/Accelerate.h>
#include <assert.h>

namespace libsignal
{
	class FFT : public FFTNode
	{
		public:
			FFT(NodeRef input = 0.0, int fft_size = 1024) :
				FFTNode(fft_size), input(input)
			{
				this->name = "fft";

				this->add_param("input", this->input);

				/*------------------------------------------------------------------------
				 * Initial FFT setup.
				 *-----------------------------------------------------------------------*/
				this->log2N = (int) log2((float) fft_size);
				this->fft_setup = vDSP_create_fftsetup(this->log2N, FFT_RADIX2);

				/*------------------------------------------------------------------------
				 * Temp buffers for FFT calculations.
				 *-----------------------------------------------------------------------*/
				this->buffer = (sample *) calloc(fft_size, sizeof(sample));
				this->buffer2 = (sample *) calloc(fft_size, sizeof(sample));

				this->hop_size = 256;

				/*------------------------------------------------------------------------
				 * To perform an FFT, we have to enqueue at least `fft_size` samples.
				 * inbuf stores our backlog, so make sure we've allocated enough space.
				 * inbuf_size records the current number of frames we have buffered.
				 *-----------------------------------------------------------------------*/
				this->inbuf = (sample *) calloc(SIGNUM_MAX_FFT_SIZE * 2, sizeof(sample));
				this->inbuf_size = 0;

				/*------------------------------------------------------------------------
				 * Hann window for overlap/add
				 *-----------------------------------------------------------------------*/
				this->window = (sample *) calloc(fft_size, sizeof(sample));
				memset(this->window, 0, sizeof(float) * this->fft_size);

				vDSP_hann_window(this->window, fft_size, vDSP_HANN_NORM);
			}

			~FFT()
			{
				vDSP_destroy_fftsetup(this->fft_setup);
				free(this->buffer);
			}

			NodeRef input;
			int hop_size;
			int log2N;
			FFTSetup fft_setup;
			sample *buffer;
			sample *buffer2;
			sample *inbuf;
			int inbuf_size;
			sample *window;

			virtual void fft(sample *in, sample *out, bool polar = true, bool do_window = true)
			{
				DSPSplitComplex buffer_split = { buffer, buffer + fft_size/2 };
				DSPSplitComplex output_split = { out, out + fft_size/2 };

				if (do_window)
					vDSP_vmul(in, 1, this->window, 1, buffer2, 1, fft_size);
				else
					memcpy(buffer2, in, fft_size * sizeof(sample));

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
				 * 1. Sending polar values
				 *-----------------------------------------------------------------------*/
				if (polar)
				{
					vDSP_ztoc(&buffer_split, 1, (DSPComplex *) buffer2, 2, fft_size/2);
					vDSP_polar(buffer2, 2, buffer, 2, fft_size/2);
					vDSP_ctoz((DSPComplex *) buffer, 2, &output_split, 1, fft_size/2);
				}

				/*------------------------------------------------------------------------
				 * 2. Sending cartesian values
				 *-----------------------------------------------------------------------*/
				else
				{
					vDSP_ztoc(&buffer_split, 1, (DSPComplex *) out, 2, fft_size/2);
				}

			}

			virtual void next(sample **out, int num_frames)
			{
				/*------------------------------------------------------------------------
				 * Append the incoming buffer onto our inbuf.
				 * Perform repeated window and FFT by stepping forward hop_size frames.
				 *-----------------------------------------------------------------------*/
				// assert(this->inbuf_size + num_frames <= this->fft_size * 2);
				memcpy(this->inbuf + this->inbuf_size, this->input->out[0], num_frames * sizeof(sample));
				this->inbuf_size += num_frames;

				/*------------------------------------------------------------------------
				 * Calculate the number of hops to perform.
				 * Each hop is stored in an output channel so we can't have > 32.
				 *-----------------------------------------------------------------------*/
				this->num_hops = (this->inbuf_size - this->fft_size) / this->hop_size;
				assert(this->num_hops <= SIGNUM_MAX_CHANNELS);

				if (this->num_hops < 0)
					this->num_hops = 0;


				for (int hop = 0; hop < this->num_hops; hop++)
				{
					this->fft(this->inbuf + (hop * this->hop_size), this->out[hop]);
				}

				int frames_processed = this->hop_size * this->num_hops;
				int frames_remaining = this->inbuf_size - frames_processed;

				memcpy(this->inbuf, this->inbuf + frames_processed, frames_remaining * sizeof(sample));
				this->inbuf_size -= frames_processed;
			}
	};

	REGISTER(FFT, "fft");
}

