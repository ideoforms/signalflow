#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class RandomImpulse : public Node
{
public:
    RandomImpulse(NodeRef frequency = 1.0,
                  signal_event_distribution_t distribution = SIGNAL_EVENT_DISTRIBUTION_UNIFORM);

    NodeRef frequency;
    signal_event_distribution_t distribution;

    virtual void process(sample **out, int num_frames);

private:
    int steps_remaining;
};

REGISTER(RandomImpulse, "random_impulse")
}
