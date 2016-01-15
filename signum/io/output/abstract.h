#pragma once

#include "../../unit.h"
#include "../../buffer.h"
#include "../../graph.h"

#include <vector>


namespace signum
{
    
    class AudioOut_Abstract : public Unit
    {
    public:
        AudioOut_Abstract(Graph *graph);
        virtual void next(sample **out, int num_samples);

        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;

        virtual void add_input(const UnitRef &unit)
		{
			inputs.push_back(unit);
			std::string input_name = "input" + std::to_string(this->inputs.size());;

			this->add_param(input_name, inputs[inputs.size() - 1]);
		}

		std::vector <UnitRef> inputs;
    };


} // namespace signum
