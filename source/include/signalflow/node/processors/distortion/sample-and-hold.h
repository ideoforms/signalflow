#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class SampleAndHold : public UnaryOpNode
{
public:
    SampleAndHold(NodeRef input = nullptr, NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void process(sample **out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    NodeRef clock;
    std::vector<sample> values;
};

REGISTER(SampleAndHold, "sample-and-hold")
}
