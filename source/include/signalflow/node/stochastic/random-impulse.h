#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class RandomImpulse : public Node
{
public:
    RandomImpulse(NodeRef frequency = 1.0,
                  signalflow_event_distribution_t distribution = SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM);

    NodeRef frequency;
    signalflow_event_distribution_t distribution;

    virtual void process(Buffer &out, int num_frames);

private:
    int steps_remaining;
};

REGISTER(RandomImpulse, "random-impulse")
}
