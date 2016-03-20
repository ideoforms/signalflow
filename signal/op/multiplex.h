#pragma once

#include "../constants.h"
#include "../node.h"
#include "../registry.h"

#include <list>

namespace libsignal
{

	class Multiplex : public Node
	{

	public:

		Multiplex() : Node()
		{
			this->name = "multiplex";
			this->no_input_automix = true;
		}

		virtual void next(sample **out, int num_frames)
		{
			int global_channel = 0; 
			for (NodeRef input : this->inputs)
			{
				for (int this_channel = 0; this_channel < input->channels_out; this_channel++)
				{
					memcpy(out[global_channel + this_channel], input->out[this_channel], num_frames * sizeof(sample));
				}
				global_channel += input->channels_out;
			}
		}

		virtual void update_channels()
		{
			this->channels_in = 0;
			for (NodeRef input : this->inputs)
			{
				this->channels_in += input->channels_out;
			}

			this->channels_out = this->channels_in;

			this->min_input_channels = this->max_input_channels = this->channels_in;
			this->min_output_channels = this->max_output_channels = this->channels_out;

			signal_debug("Node %s set num_out_channels to %d", this->name.c_str(), this->channels_out);
		}

		virtual void add_input(NodeRef input)
		{
			this->inputs.push_back(input);
            std::string input_name = "input" + std::to_string(this->inputs.size());;
            this->add_param(input_name, inputs.back());
		}

		std::list <NodeRef> inputs;
	};

	REGISTER(Multiplex, "multiplex");

}

