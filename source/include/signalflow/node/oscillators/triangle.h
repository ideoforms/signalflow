#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Triangle : public Node
{
public:
    Triangle(NodeRef frequency = 440);

    NodeRef frequency;

    float phase[SIGNAL_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);
};

REGISTER(Triangle, "triangle")
}
