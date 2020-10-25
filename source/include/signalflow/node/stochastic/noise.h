#pragma once

#include "signalflow/node/node.h"

typedef enum
{
    SIGNALFLOW_DISTRIBUTION_UNIFORM,
    SIGNALFLOW_DISTRIBUTION_GAUSSIAN,
    SIGNALFLOW_DISTRIBUTION_EXPONENTIAL,
    SIGNALFLOW_DISTRIBUTION_CAUCHY,
    SIGNALFLOW_DISTRIBUTION_BETA,
    SIGNALFLOW_DISTRIBUTION_GAMMA,
    SIGNALFLOW_DISTRIBUTION_LEVY

} SIGNALFLOW_NOISE_DISTRIBUTION;

namespace signalflow
{
class Noise : public Node
{
public:
    Noise(NodeRef frequency = 0.0, bool interpolate = true, NodeRef min = 0.0, NodeRef max = 1.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef frequency;
    NodeRef min;
    NodeRef max;
    bool interpolate;

    std::vector<sample> value;
    std::vector<int> steps_remaining;
    std::vector<float> step_change;
};

REGISTER(Noise, "noise")
}
