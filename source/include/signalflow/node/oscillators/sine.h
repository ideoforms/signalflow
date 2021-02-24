#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class SineOscillator : public Node
{
public:
    SineOscillator(NodeRef frequency = 440);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void alloc() override;

    NodeRef frequency;

private:
    std::vector<float> phase;
};

REGISTER(SineOscillator, "sine")
}
