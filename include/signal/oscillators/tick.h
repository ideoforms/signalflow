#pragma once 

#include "signal/node.h"

namespace libsignal
{
    class Tick : public Node
    {
    public:
        Tick(NodeRef frequency = 1.0);

        virtual void process(sample **out, int num_frames);

        NodeRef frequency;
        int steps_remaining;

    };

    REGISTER(Tick, "tick");
}
