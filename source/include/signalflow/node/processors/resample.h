#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class Resample : public UnaryOpNode
{
public:
    Resample(NodeRef input = 0, NodeRef sample_rate = 22050, NodeRef bit_rate = 8);

    NodeRef sample_rate;
    NodeRef bit_rate;

    virtual void process(sample **out, int num_frames);

private:
    float phase;
    float phase_last;
    sample sample_last[SIGNAL_MAX_CHANNELS];
};

REGISTER(Resample, "resample")
}
