#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class RandomGaussian : public Node
{
public:
    RandomGaussian(NodeRef mean = 0.0, NodeRef sigma = 0.0, NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef mean;
    NodeRef sigma;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomGaussian, "random-gaussian")
}
