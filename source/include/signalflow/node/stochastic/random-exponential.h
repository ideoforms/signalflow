#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{
class RandomExponential : public StochasticNode
{
public:
    RandomExponential(NodeRef scale = 0.0, NodeRef clock = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef scale;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomExponential, "random-exponential")
}
