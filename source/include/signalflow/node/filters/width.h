#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Width : public UnaryOpNode
{
public:
    Width(NodeRef input = 0, NodeRef width = 1);
    virtual void process(sample **out, int num_frames);

    NodeRef width;
};

REGISTER(Width, "width")
}
