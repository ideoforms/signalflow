#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{
class RandomImpulse : public StochasticNode
{
public:
    RandomImpulse(NodeRef frequency = 1.0,
                  signalflow_event_distribution_t distribution = SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM,
                  NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef frequency;
    signalflow_event_distribution_t distribution;

    std::vector<int> steps_remaining;
};

REGISTER(RandomImpulse, "random-impulse")
}
