#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Sine : public Node
{
public:
    Sine(NodeRef frequency = 440);

    virtual void process(sample **out, int num_frames) override;
    virtual void alloc() override;

    NodeRef frequency;

private:
    std::vector<float> phase;
};

REGISTER(Sine, "sine")
}
