#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs Brownian noise between min/max, with a mean change of delta between samples.
 * If a clock is passed, only generates a new value on a clock tick.
 * To generate and hold an unchanging random value, pass clock=0.
 *---------------------------------------------------------------------------------*/
class RandomBrownian : public StochasticNode
{
public:
    RandomBrownian(NodeRef min = -1.0, NodeRef max = 1.0, NodeRef delta = 0.01, NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef min;
    NodeRef max;
    NodeRef delta;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomBrownian, "random-brownian")
}
