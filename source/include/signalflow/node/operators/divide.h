#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Divide each sample of a by each sample of b.
 * Can also be written as a / b
 *---------------------------------------------------------------------------------*/
class Divide : public BinaryOpNode
{

public:
    Divide(NodeRef a = 1, NodeRef b = 1);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Divide, "divide")

}
