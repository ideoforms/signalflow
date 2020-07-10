#pragma once

#include "signalflow/node/node.h"

typedef enum
{
    SIGNAL_DISTRIBUTION_UNIFORM,
    SIGNAL_DISTRIBUTION_GAUSSIAN,
    SIGNAL_DISTRIBUTION_EXPONENTIAL,
    SIGNAL_DISTRIBUTION_CAUCHY,
    SIGNAL_DISTRIBUTION_BETA,
    SIGNAL_DISTRIBUTION_GAMMA,
    SIGNAL_DISTRIBUTION_LEVY

} SIGNAL_NOISE_DISTRIBUTION;

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

    sample value[SIGNAL_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);

private:
    int steps_remaining[SIGNAL_MAX_CHANNELS];
    float step_change[SIGNAL_MAX_CHANNELS];
};

REGISTER(Noise, "noise")
}
