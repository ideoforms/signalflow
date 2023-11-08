#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Subtract each sample of b from each sample of a.
 * Can also be written as a - b
 *---------------------------------------------------------------------------------*/
class Subtract : public BinaryOpNode
{

public:
    Subtract(NodeRef a = 0, NodeRef b = 0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Subtract, "subtract")

}
