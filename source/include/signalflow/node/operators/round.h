#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Round : public UnaryOpNode
{

public:
    Round(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Round, "round")

}
