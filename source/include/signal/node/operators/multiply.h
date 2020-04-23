#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{

class Multiply : public BinaryOpNode
{

public:
    Multiply(NodeRef a = 1.0, NodeRef b = 1.0);

    virtual void process(sample **out, int num_frames);
};

REGISTER(Multiply, "multiply")

}
