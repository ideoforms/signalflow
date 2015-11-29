#include "../constants.h"
#include "../unit.h"

namespace signum::op
{

class Divide : public BinaryOpUnit
{

public:

	Divide(UnitRef a, UnitRef b) : BinaryOpUnit(a, b)
	{
		this->name = "divide";
	}

	virtual void next(sample **out, int num_frames)
	{
		UnitRef a = this->inputs[0];
		UnitRef b = this->inputs[1];
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				out[channel][frame] = a->out[channel][frame] / b->out[channel][frame];
			}
		}
	}
};

}
