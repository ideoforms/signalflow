#include "moog.h" 

#include "../gen/constant.h"

#include <stdlib.h>

namespace libsignal
{

void MoogVCF::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < num_output_channels; channel++)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			float cutoff = this->cutoff->out[channel][frame];
			float resonance = this->resonance->out[channel][frame];
			float f = cutoff * 1.16; 
			float fb = resonance * (1.0 - 0.15 * f * f);

			/*------------------------------------------------------------------------
			 * Low-pass filter
			 *-----------------------------------------------------------------------*/
			float input = this->input->out[channel][frame];

			input -= out4[channel] * fb;
			input *= 0.35013 * (f*f)*(f*f);
			out1[channel] = input + 0.3 * in1[channel] + (1 - f) * out1[channel]; // Pole 1
			in1[channel]  = input;
			out2[channel] = out1[channel] + 0.3 * in2[channel] + (1 - f) * out2[channel];  // Pole 2
			in2[channel]  = out1[channel];
			out3[channel] = out2[channel] + 0.3 * in3[channel] + (1 - f) * out3[channel];  // Pole 3
			in3[channel]  = out2[channel];
			out4[channel] = out3[channel] + 0.3 * in4[channel] + (1 - f) * out4[channel];  // Pole 4
			in4[channel]  = out3[channel];

			this->out[channel][frame] = out4[channel];
		}
	}
}

}
