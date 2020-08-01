#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Saw : public Node
{
public:
    Saw(NodeRef frequency = 440);

    NodeRef frequency;

    float phase[SIGNALFLOW_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);
};

REGISTER(Saw, "saw")
}
