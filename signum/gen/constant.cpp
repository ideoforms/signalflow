#include "constant.h"

namespace signum
{

Constant::Constant(sample value) : Unit()
{
	this->value = value;
	this->name = "constant";
}

void Constant::next(sample **out, int num_frames)
{
	for (int frame = 0; frame < num_frames; frame++)
	{
		for (int channel = 0; channel < this->channels_out; channel++)
		{
			out[channel][frame] = this->value;
		}
	}
}

}
