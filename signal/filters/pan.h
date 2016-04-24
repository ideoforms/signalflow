#pragma once

#include "../node.h"

namespace libsignal
{
	class Pan : public Node
	{
		public:
			Pan(int channels = 2, NodeRef input = 2, NodeRef pan = 0.5) :
				input(input), pan(pan)
			{
				this->num_input_channels = 1;
				this->num_output_channels = channels;

				this->min_input_channels = this->max_input_channels = 1;
				this->min_output_channels = this->max_output_channels = channels;

				this->add_param("input", this->input);
				this->add_param("pan", this->pan);

				this->name = "pan";
			}

			NodeRef input;
			NodeRef pan;

			virtual void next(sample **out, int num_frames);
	};

	REGISTER(Pan, "pan");
}
