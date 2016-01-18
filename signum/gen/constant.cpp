#include "constant.h"

namespace signum
{

Constant::Constant(sample value) : Unit()
{
	this->value = value;
	this->name = "constant";

	this->min_input_channels = 0;
	this->max_input_channels = 0;
}

void Constant::next(sample **out, int num_frames)
{
	for (int channel = 0; channel < this->channels_out; channel++)
	{
		#if __APPLE__
		vDSP_vfill(&(this->value), out[channel], 1, num_frames);
		#else
		for (int frame = 0; frame < num_frames; frame++)
			out[channel][frame] = this->value;
		#endif
	}
}

}
