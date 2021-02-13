#pragma once

#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{
class PinkNoise : public StochasticNode
{
public:
    PinkNoise(float low_cutoff = 20.0, float high_cutoff = 20000.0, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    std::vector<std::vector<sample>> value;
    std::vector<std::vector<int>> steps_remaining;

    int num_octaves;
    int initial_octave;
};

REGISTER(PinkNoise, "pinknoise")
}
