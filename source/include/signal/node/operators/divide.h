#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{

class Divide : public BinaryOpNode
{

public:
    Divide(NodeRef a = 1, NodeRef b = 1);

    virtual void process(sample **out, int num_frames);
};

REGISTER(Divide, "divide")

}
