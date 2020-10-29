#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Tanh : public UnaryOpNode
{

public:
    Tanh(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Tanh, "tanh")

}
