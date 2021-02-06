#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class PinkNoise : public Node
{
public:
    PinkNoise();

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    std::vector<std::vector<sample>> value;
    std::vector<std::vector<int>> steps_remaining;
};

REGISTER(PinkNoise, "pinknoise")
}
