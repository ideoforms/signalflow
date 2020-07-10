#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Logistic : public Node
{
public:
    Logistic(NodeRef chaos = 3.7, NodeRef frequency = 0.0);

    NodeRef chaos;
    NodeRef frequency;

    virtual void process(sample **out, int num_frames);

private:
    sample value[SIGNAL_MAX_CHANNELS];
    int steps_remaining[SIGNAL_MAX_CHANNELS];
};

REGISTER(Logistic, "logistic")
}
