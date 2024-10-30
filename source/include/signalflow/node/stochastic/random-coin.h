#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Flip a coin with the given probability.
 * If a clock is passed, only picks a new value on a clock tick.
 * To generate and hold an unchanging random value, pass clock=0.
 *---------------------------------------------------------------------------------*/
class RandomCoin : public StochasticNode
{
public:
    RandomCoin(NodeRef probability = 0.5, NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef probability;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomCoin, "random-coin")
}
