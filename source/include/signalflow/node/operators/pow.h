#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Pow : public BinaryOpNode
{

public:
    Pow(NodeRef a = 0, NodeRef b = 0);

    virtual void process(sample **out, int num_frames);
};

REGISTER(Pow, "pow")

}
