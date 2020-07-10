#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class LinExp : public UnaryOpNode
{

public:
    LinExp(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(sample **out, int num_frames);

    NodeRef a, b, c, d;
};

class Scale : public UnaryOpNode
{

public:
    Scale(NodeRef input = 0, NodeRef a = 0, NodeRef b = 1, NodeRef c = 1, NodeRef d = 10);

    virtual void process(sample **out, int num_frames);

    NodeRef a, b, c, d;
};

REGISTER(LinExp, "scale-lin-exp")
REGISTER(Scale, "scale")
}
