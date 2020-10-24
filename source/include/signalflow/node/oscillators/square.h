#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Square : public Node
{
public:
    Square(NodeRef frequency = 440, NodeRef width = 0.5);

    NodeRef frequency;
    NodeRef width;

    virtual void alloc() override;
    virtual void process(sample **out, int num_frames) override;

private:
    std::vector<float> phase;
};

REGISTER(Square, "square")
}
