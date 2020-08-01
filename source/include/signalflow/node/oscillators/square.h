#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Square : public Node
{
public:
    Square(NodeRef frequency = 440, NodeRef width = 0.5);

    NodeRef frequency;
    NodeRef width;

    float phase[SIGNALFLOW_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);
};

REGISTER(Square, "square")
}
