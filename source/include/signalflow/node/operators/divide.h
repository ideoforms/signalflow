#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Divide : public BinaryOpNode
{

public:
    Divide(NodeRef a = 1, NodeRef b = 1);

    virtual void process(sample **out, int num_frames);
};

REGISTER(Divide, "divide")

}
