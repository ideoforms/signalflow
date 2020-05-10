#pragma once

#include "signal/node/node.h"

namespace libsignal
{

class Impulse : public Node
{
public:
    Impulse(NodeRef frequency = 1.0);

    virtual void process(sample **out, int num_frames);

    NodeRef frequency;
    int steps_remaining;
};

REGISTER(Impulse, "impulse")

}
