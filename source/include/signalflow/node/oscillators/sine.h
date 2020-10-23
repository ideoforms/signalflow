#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Sine : public Node
{
public:
    Sine(NodeRef frequency = 440);

    virtual void process(sample **out, int num_frames) override;
    virtual void allocate_memory(int output_buffer_count) override;

    NodeRef frequency;

private:
    std::vector<float> phase;
};

REGISTER(Sine, "sine")
}
