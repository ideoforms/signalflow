#include "moog.h" 

#include "../gen/constant.h"

#include <stdlib.h>

namespace libsignal
{

void MoogVCF::next(sample **out, int num_frames)
{
	int channel = 0;
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

			input -= out4 * fb;
			input *= 0.35013 * (f*f)*(f*f);
			out1 = input + 0.3 * in1 + (1 - f) * out1; // Pole 1
			in1  = input;
			out2 = out1 + 0.3 * in2 + (1 - f) * out2;  // Pole 2
			in2  = out1;
			out3 = out2 + 0.3 * in3 + (1 - f) * out3;  // Pole 3
			in3  = out2;
			out4 = out3 + 0.3 * in4 + (1 - f) * out4;  // Pole 4
			in4  = out3;

			this->out[channel][frame] = out4;
		}
}

}
