#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Generate an random value following the exponential distribution.
 * If a clock is passed, only picks a new value on a clock tick.
 * To generate and hold an unchanging random value, pass clock=0.
 *---------------------------------------------------------------------------------*/
class RandomExponentialDist : public StochasticNode
{
public:
    RandomExponentialDist(NodeRef scale = 0.0, NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef scale;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomExponentialDist, "random-exponential-dist")
}
