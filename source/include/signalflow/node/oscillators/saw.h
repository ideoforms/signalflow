#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Saw : public Node
{
public:
    Saw(NodeRef frequency = 440);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef frequency;

private:
    std::vector<float> phase;
};

REGISTER(Saw, "saw")
}
