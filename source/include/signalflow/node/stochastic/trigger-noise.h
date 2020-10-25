#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class TriggerNoise : public Node
{
public:
    TriggerNoise(NodeRef min = 0.0, NodeRef max = 1.0, NodeRef clock = 0.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef min;
    NodeRef max;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(TriggerNoise, "trigger-noise")
}
