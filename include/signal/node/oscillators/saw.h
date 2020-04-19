#pragma once 

#include "signal/node/node.h"

namespace libsignal
{
    class Saw : public Node
    {
    public:
        Saw(NodeRef frequency = 440);

        NodeRef frequency;

        float phase[SIGNAL_MAX_CHANNELS];

        virtual void process(sample **out, int num_frames);
    };

    REGISTER(Saw, "saw");
}
