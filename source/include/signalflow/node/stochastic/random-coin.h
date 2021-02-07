#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class RandomCoin : public Node
{
public:
    RandomCoin(NodeRef probability = 0.5, NodeRef clock = nullptr);

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
