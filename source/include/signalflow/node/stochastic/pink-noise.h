#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class PinkNoise : public Node
{
public:
    PinkNoise(float low_cutoff = 20.0, float high_cutoff = 20000.0);

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
