#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class SawOscillator : public Node
{
public:
    SawOscillator(NodeRef frequency = 440);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef frequency;

private:
    std::vector<float> phase;
};

REGISTER(SawOscillator, "saw")
}
