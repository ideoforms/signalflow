#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class Resample : public UnaryOpNode
{
public:
    Resample(NodeRef input = 0, NodeRef sample_rate = 22050, NodeRef bit_rate = 8);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef sample_rate;
    NodeRef bit_rate;

    float phase;
    float phase_last;
    std::vector<sample> sample_last;
};

REGISTER(Resample, "resample")
}
