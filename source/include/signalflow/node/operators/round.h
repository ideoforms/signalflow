#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class RoundToScale : public UnaryOpNode
{

public:
    RoundToScale(NodeRef a);

    virtual void process(Buffer &out, int num_frames);
};

}
