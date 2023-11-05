#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class SawOscillator : public Node
{
public:
    SawOscillator(NodeRef frequency = 440, NodeRef phase = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

    NodeRef frequency;
    NodeRef phase;
    NodeRef reset;

private:
    std::vector<float> phase_offset;
};

REGISTER(SawOscillator, "saw")
}
