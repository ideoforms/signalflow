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

    NodeRef frequency;
    bool interpolate;

    NodeRef min;
    NodeRef max;

    sample value[SIGNALFLOW_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);

private:
    int steps_remaining[SIGNALFLOW_MAX_CHANNELS];
    float step_change[SIGNALFLOW_MAX_CHANNELS];
};

REGISTER(Noise, "noise")
}
