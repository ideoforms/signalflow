#include "../constants.h"
#include "../unit.h"

namespace signum::op
{

	class Divide : public BinaryOpUnit
	{

	public:

		Divide(UnitRef a = 1, UnitRef b = 1) : BinaryOpUnit(a, b)
		{
			this->name = "divide";
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				for (int channel = 0; channel < this->channels_out; channel++)
				{
					out[channel][frame] = input0->out[channel][frame] / input1->out[channel][frame];
				}
			}
		}
	};

	REGISTER(Divide, "divide");

}
