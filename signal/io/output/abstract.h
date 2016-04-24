#pragma once

#include "../../node.h"
#include "../../buffer.h"
#include "../../graph.h"

#include <list>


namespace libsignal
{
    
    class AudioOut_Abstract : public Node
    {
    public:
        AudioOut_Abstract(Graph *graph);
        virtual void process(sample **out, int num_samples);

        int sample_rate = 0;

        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;

        virtual void add_input(NodeRef node)
		{
			inputs.push_back(node);
			std::string input_name = "input" + std::to_string(this->inputs.size());;

			this->Node::add_input(input_name, inputs.back());
		}

		std::list <NodeRef> inputs;
    };

} // namespace libsignal
