#include "../constants.h"
#include "../unit.h"

namespace signum::op
{

class Add : public BinaryOpUnit
{

public:

	Add(UnitRef a, UnitRef b) : BinaryOpUnit(a, b)
	{
		this->name = "add";
	}

	virtual void next(sample **out, int num_frames)
	{
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				out[channel][frame] = input0->out[channel][frame] + input1->out[channel][frame];
			}
		}
	}
};

}
