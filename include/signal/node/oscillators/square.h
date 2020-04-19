#pragma once 

#include "signal/node/node.h"

namespace libsignal
{
    class Square : public Node
    {
    public:
        Square(NodeRef frequency = 440, NodeRef width = 0.5);

        NodeRef frequency;
        NodeRef width;

        float phase[SIGNAL_MAX_CHANNELS];

        virtual void process(sample **out, int num_frames);
    };

    REGISTER(Square, "square");
}
