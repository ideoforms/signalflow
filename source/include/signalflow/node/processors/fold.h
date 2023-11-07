#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Fold the input beyond min/max, reflecting the excess back.
 *---------------------------------------------------------------------------------*/
class Fold : public UnaryOpNode
{
public:
    Fold(NodeRef input = nullptr, NodeRef min = -1.0, NodeRef max = 1.0);

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef min;
    NodeRef max;
};

REGISTER(Fold, "fold")
}
