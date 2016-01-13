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
    };


} // namespace signum
