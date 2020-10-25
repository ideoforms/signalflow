#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{

class Impulse : public Node
{
public:
    Impulse(NodeRef frequency = 1.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef frequency;

private:
    std::vector<int> steps_remaining;
};

REGISTER(Impulse, "impulse")

}
