#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class SawOscillator : public Node
{
public:
    SawOscillator(NodeRef frequency = 440, NodeRef phase = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef frequency;
    NodeRef phase;

private:
    std::vector<float> phase_offset;
};

REGISTER(SawOscillator, "saw")
}
