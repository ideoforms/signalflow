#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"

namespace signum
{

	class Multiply : public BinaryOpUnit
	{

	public:

		Multiply(UnitRef a = 1.0, UnitRef b = 1.0) : BinaryOpUnit(a, b)
		{
			this->name = "multiply";
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				#ifdef __APPLE__

					vDSP_vmul(input0->out[channel], 1, input1->out[channel], 1, out[channel], 1, num_frames);

				#else

					for (int frame = 0; frame < num_frames; frame++)
						out[channel][frame] = input0->out[channel][frame] * input1->out[channel][frame];

				#endif
			}
		}
	};

	REGISTER(Multiply, "multiply");

}

