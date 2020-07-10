#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{

class Impulse : public Node
{
public:
    Impulse(NodeRef frequency = 1.0);

    virtual void process(sample **out, int num_frames);

    NodeRef frequency;
    std::vector<int> steps_remaining;
};

REGISTER(Impulse, "impulse")

}
