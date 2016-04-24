#pragma once

#include "../node.h"
#include "../constants.h"

namespace libsignal
{
	class Freeze : public UnaryOpNode
	{
		public:
			Freeze(NodeRef input = 0) :
				UnaryOpNode(input)
			{
				this->buffer = new Buffer(1, 2048);

				this->name = "freeze";

				this->frozen = false;
				this->trig = false;
			}

			Buffer *buffer;
			bool frozen;
			bool trig;

			virtual void process(sample **out, int num_frames)
			{
				if (this->trig || !this->frozen)
				{
					memcpy(this->buffer->data[0], this->input->out[0], sizeof(sample) * num_frames);
					this->trig = false;
				}

				memcpy(out[0], this->buffer->data[0], sizeof(sample) * num_frames);
			}

			virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0)
			{
				this->frozen = true;
				this->trig = true;
			}
	};

	REGISTER(Freeze, "freeze");
}
