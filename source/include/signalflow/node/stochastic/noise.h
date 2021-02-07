#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Noise : public Node
{
public:
    Noise(NodeRef frequency = 0.0,
          NodeRef min = -1.0, NodeRef max = 1.0,
          bool interpolate = true, bool random_interval = true);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef frequency;
    NodeRef min;
    NodeRef max;
    bool interpolate;
    bool random_interval;

    std::vector<sample> value;
    std::vector<int> steps_remaining;
    std::vector<float> step_change;
};

REGISTER(Noise, "noise")
}
