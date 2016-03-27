#include "../constants.h"
#include "../node.h"

namespace libsignal
{

	class Add : public BinaryOpNode
	{

	public:

		Add(NodeRef a = 0, NodeRef b = 0) : BinaryOpNode(a, b)
		{
			this->name = "add";
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				for (int channel = 0; channel < this->num_output_channels; channel++)
				{
					out[channel][frame] = input0->out[channel][frame] + input1->out[channel][frame];
				}
			}
		}

	};

	REGISTER(Add, "add");

}
