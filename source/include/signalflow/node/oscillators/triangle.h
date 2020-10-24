#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Triangle : public Node
{
public:
    Triangle(NodeRef frequency = 440);

    NodeRef frequency;

    virtual void alloc() override;
    virtual void process(sample **out, int num_frames) override;

private:
    std::vector<float> phase;
};

REGISTER(Triangle, "triangle")
}
