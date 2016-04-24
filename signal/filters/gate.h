#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class Gate : public UnaryOpNode
	{
		public:
			Gate(NodeRef input = 0.0, NodeRef threshold = 0.1, NodeRef attack = 0.0, NodeRef hold = 0.0, NodeRef release = 0.0) :
				UnaryOpNode(input), threshold(threshold), attack(attack), hold(hold), release(release)
			{
				this->open = false;
				this->openTime = 0;
				this->level = 0.0;

				this->name = "gate";
				this->add_param("threshold", this->threshold);
				this->add_param("attack", this->attack);
				this->add_param("hold", this->hold);
				this->add_param("release", this->release);
			}

			NodeRef sample_rate;
			NodeRef bit_rate;

			bool open;
			float openTime;
			float level;

			NodeRef threshold;

			// TODO: Implement attack, hold, release (based on current graph sample rate)
			NodeRef attack;
			NodeRef hold;
			NodeRef release;

			virtual void process(sample **out, int num_frames)
			{
				for (int frame = 0; frame < num_frames; frame++)
				{
					float amp = 0.0;
					for (int channel = 0; channel < this->num_input_channels; channel++)
						amp += this->input->out[channel][frame];
					amp /= this->num_input_channels;

					float threshold = this->threshold->out[0][frame];

					if (amp > threshold)
					{
						open = true;
						level = 1.0;
					}
					else
					{
						open = false;
						level = 0.0;
					}

					for (int channel = 0; channel < this->num_output_channels; channel++)
					{
						this->out[channel][frame] = this->input->out[channel][frame] * level;
					}
				}
			}
	};

	REGISTER(Gate, "gate");
}
