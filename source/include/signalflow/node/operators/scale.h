#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class ScaleLinExp : public UnaryOpNode
{

public:
    ScaleLinExp(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(Buffer &out, int num_frames);

    NodeRef a, b, c, d;
};

class ScaleLinLin : public UnaryOpNode
{

public:
    ScaleLinLin(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(Buffer &out, int num_frames);

    NodeRef a, b, c, d;
};

REGISTER(ScaleLinExp, "scale-lin-exp")
REGISTER(ScaleLinLin, "scale-lin-lin")
}
