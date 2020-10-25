#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Logistic : public Node
{
public:
    Logistic(NodeRef chaos = 3.7, NodeRef frequency = 0.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef chaos;
    NodeRef frequency;

    std::vector<sample> value;
    std::vector<int> steps_remaining;
};

REGISTER(Logistic, "logistic")
}
