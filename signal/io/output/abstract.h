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
        virtual void next(sample **out, int num_samples);

        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;

        virtual void add_input(NodeRef unit)
		{
			inputs.push_back(unit);
			std::string input_name = "input" + std::to_string(this->inputs.size());;

			this->add_param(input_name, inputs.back());
		}

		std::list <NodeRef> inputs;
    };

} // namespace libsignal
