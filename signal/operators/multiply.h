#pragma once

#include "../constants.h"
#include "../node.h"
#include "../registry.h"

namespace libsignal
{

	class Multiply : public BinaryOpNode
	{

	public:

		Multiply(NodeRef a = 1.0, NodeRef b = 1.0) : BinaryOpNode(a, b)
		{
			this->name = "multiply";
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int channel = 0; channel < this->num_output_channels; channel++)
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

