#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Round the input to the nearest integer value.
 *---------------------------------------------------------------------------------*/
class Round : public UnaryOpNode
{

public:
    Round(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Round, "round")

}
