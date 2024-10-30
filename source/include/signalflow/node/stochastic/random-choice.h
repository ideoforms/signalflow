#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Pick a random value from the given array.
 * If a clock is passed, only picks a new value on a clock tick.
 * To generate and hold an unchanging random value, pass clock=0.
 *---------------------------------------------------------------------------------*/
class RandomChoice : public StochasticNode
{
public:
    RandomChoice(std::vector<float> values = std::vector<float>(),
                 NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    std::vector<sample> values;
    std::vector<sample> value;

    NodeRef clock;
};

REGISTER(RandomChoice, "random-choice")
}
