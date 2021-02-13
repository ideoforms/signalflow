#pragma once

#include "signalflow/node/node.h"

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <sys/time.h>

namespace signalflow
{
class WhiteNoise : public Node
{
public:
    WhiteNoise(NodeRef frequency = 0.0,
               NodeRef min = -1.0, NodeRef max = 1.0,
               bool interpolate = true, bool random_interval = true,
               NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef frequency;
    NodeRef min;
    NodeRef max;
    bool interpolate;
    bool random_interval;
    NodeRef reset;

    std::vector<sample> value;
    std::vector<int> steps_remaining;
    std::vector<float> step_change;

    unsigned long int seed;
    gsl_rng *rng;
};

REGISTER(WhiteNoise, "white-noise")
}
