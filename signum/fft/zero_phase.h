#pragma once

#include "../unit.h"
#include "../constants.h"

namespace signum
{
	class FFTZeroPhase : public UnaryOpUnit
	{
		public:
			FFTZeroPhase(UnitRef input = 0) :
				UnaryOpUnit(input)
			{
				this->name = "zero_phase";
			}

			virtual void next(sample **out, int num_frames)
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
