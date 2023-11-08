#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs a to the power of b, per sample.
 * Can also be written as a ** b
 *---------------------------------------------------------------------------------*/
class Pow : public BinaryOpNode
{

public:
    Pow(NodeRef a = 0, NodeRef b = 0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Pow, "pow")

}
