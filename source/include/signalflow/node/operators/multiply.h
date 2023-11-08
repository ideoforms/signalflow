#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Multiply each sample of a by each sample of b.
 * Can also be written as a * b
 *---------------------------------------------------------------------------------*/
class Multiply : public BinaryOpNode
{

public:
    Multiply(NodeRef a = 1.0, NodeRef b = 1.0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Multiply, "multiply")

}
