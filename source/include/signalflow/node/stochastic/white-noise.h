#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Generates whitenoise between min/max. If frequency is zero, generates at
 * audio rate. For frequencies lower than audio rate, interpolate applies linear
 * interpolation between values, and random_interval specifies whether new
 * random values should be equally-spaced or randomly-spaced.
 *---------------------------------------------------------------------------------*/
class WhiteNoise : public StochasticNode
{
public:
    WhiteNoise(NodeRef frequency = 0.0,
               NodeRef min = -1.0, NodeRef max = 1.0,
               bool interpolate = true, bool random_interval = true,
               NodeRef reset = nullptr);

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

REGISTER(WhiteNoise, "white-noise")
}
