#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class RandomExponential : public Node
{
public:
    RandomExponential(NodeRef scale = 0.0, NodeRef clock = nullptr);

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
