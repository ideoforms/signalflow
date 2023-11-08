#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Add each sample of a to each sample of b.
 * Can also be written as a + b
 *---------------------------------------------------------------------------------*/
class Add : public BinaryOpNode
{

public:
    Add(NodeRef a = 0, NodeRef b = 0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Add, "add")

}
