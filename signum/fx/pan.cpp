#include "pan.h" 

namespace signum::fx
{

void Pan::next(frame in, frame out)
{
	sample pan = this->pan->next();

	if (this->num_output_channels == 2)
	{
		out[0] = in[0] * (1.0 - pan);
		out[1] = in[0] * pan;
	}
	else if (this->num_output_channels == 4)
	{
		// TODO calculate channels frames based on pan as angle in radians
	}
}


}
