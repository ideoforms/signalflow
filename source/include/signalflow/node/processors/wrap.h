#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Wrap the input beyond `min`/`max`.
 *---------------------------------------------------------------------------------*/
class Wrap : public UnaryOpNode
{
public:
    Wrap(NodeRef input = nullptr, NodeRef min = -1.0, NodeRef max = 1.0);

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef min;
    NodeRef max;
};

REGISTER(Wrap, "wrap")
}
