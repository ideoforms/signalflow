#pragma once 

#include "../node.h"

namespace libsignal
{
	class Tick : public Node
	{
	public:
		Tick(NodeRef frequency = 1.0) :
			frequency(frequency)
		{
			this->name = "tick";
			this->steps_remaining = 0;
			this->add_param("frequency", this->frequency);
		}

		virtual void next(sample **out, int num_frames)
		{
			for (int frame = 0; frame < num_frames; frame++)
			{
				sample rv = 0;
				if (this->steps_remaining <= 0)
				{
					float freq_in = this->frequency->out[0][frame];
					if (freq_in > 0)
					{
						this->steps_remaining = this->graph->sample_rate / this->frequency->out[0][frame];
						rv = 1;
					}
				}
				else
				{
					this->steps_remaining--;
				}

				this->out[0][frame] = rv;
			}
		}

		NodeRef frequency;
		int steps_remaining;

	};

	REGISTER(Tick, "tick");
}
