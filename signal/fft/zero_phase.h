#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class FFTZeroPhase : public UnaryOpNode
	{
		public:
			FFTZeroPhase(NodeRef input = 0) :
				UnaryOpNode(input)
			{
				this->name = "zero_phase";
			}

			virtual void process(sample **out, int num_frames)
			{
				for (int i = 0; i < num_frames; i++)
				{
					if (i > 0 && i < num_frames / 2)
					{
						if (i < num_frames / 4)
							out[0][i] = input->out[0][i];
						else
							out[0][i] = input->out[0][i];
					}
					else
						out[0][i] = input->out[0][i] * 0.1;
				}
			}
	};

	REGISTER(FFTZeroPhase, "zero_phase");
}
