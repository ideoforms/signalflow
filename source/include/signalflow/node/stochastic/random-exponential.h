#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Generate an random exponential value between min/max.
 * If a clock is passed, only picks a new value on a clock tick.
 * To generate and hold an unchanging random value, pass clock=0.
 *---------------------------------------------------------------------------------*/
class RandomExponential : public StochasticNode
{
public:
    RandomExponential(NodeRef min = 0.001, NodeRef max = 1.0,
                      NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef min;
    NodeRef max;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomExponential, "random-exponential")
}
