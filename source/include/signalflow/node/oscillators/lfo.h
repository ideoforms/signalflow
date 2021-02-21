#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class LFO : public Node
{
public:
    LFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0);
    virtual void alloc() override;

protected:
    std::vector<float> phase;
    NodeRef frequency;
    NodeRef min;
    NodeRef max;
};

}
