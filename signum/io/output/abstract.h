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
        virtual int init() = 0;
        virtual int start() = 0;
        virtual int close() = 0;
        virtual void next(sample **out, int num_samples) = 0;
        
        virtual void add_input(const UnitRef &input)
        {
            inputs.push_back(input);
        }
    };


} // namespace signum