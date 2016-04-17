#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class FFTPhaseVocoder : public FFTOpNode
	{
		public:
			FFTPhaseVocoder(NodeRef input = nullptr) :
				FFTOpNode(input)
			{
				this->name = "fft_phase_vocoder";

				this->phase_buffer     = (sample *) calloc(this->num_bins, sizeof(sample));
				this->phase_deriv      = (sample *) calloc(this->num_bins, sizeof(sample));
				this->magnitude_buffer = (sample *) calloc(this->num_bins, sizeof(sample));

				this->frozen = false;
			}

			sample *magnitude_buffer;
			sample *phase_buffer;
			sample *phase_deriv;
			bool frozen;

			virtual void next(sample **out, int num_frames)
			{
				FFTNode *fftnode = (FFTNode *) this->input.get();
				this->num_hops = fftnode->num_hops; 

				bool triggered = this->triggered();
				if (!frozen && triggered)
					frozen = true;

				for (int hop = 0; hop < this->num_hops; hop++)
				{
					/*------------------------------------------------------------------------
					 * Rather than num_frames here, we need to iterate over fft_size frames
					 *  - as each block contains a whole fft of samples.
					 *-----------------------------------------------------------------------*/
					for (int frame = 0; frame < this->fft_size; frame++)
					{
						if (frozen)
						{
							if (frame < this->num_bins)
							{
								out[hop][frame] = magnitude_buffer[frame];
								// if (frame == 0)
								// 	out[hop][frame] = 0.0;
							}
							else
							{
								// copy magnitudes in to out;
								// for phases, increase by phase_deriv per hop
								int phase_index = frame - num_bins;
								phase_buffer[phase_index] = phase_buffer[phase_index] + phase_deriv[phase_index];
								if (phase_buffer[phase_index] >= M_PI)
									phase_buffer[phase_index] -= 2.0 * M_PI;
								out[hop][frame] = phase_buffer[phase_index];
							}
						}
						else
						{
							// copy in to out
							out[hop][frame] = input->out[hop][frame];
						}
					}
				}

				int last_hop = this->num_hops - 1;
				if ((last_hop >= 1) && (!frozen || triggered))
				{
					FFTNode *fftin = (FFTNode *) input.get();
					for (int frame = 0; frame < this->num_bins; frame++)
					{
						this->phase_deriv[frame]      = fftin->phases[last_hop][frame] - fftin->phases[last_hop - 1][frame];
						this->phase_buffer[frame]     = fftin->phases[last_hop][frame];
						this->magnitude_buffer[frame] = fftin->magnitudes[last_hop][frame];
					}
				}
			}
	};

	REGISTER(FFTPhaseVocoder, "fft_phase_vocoder");
}
