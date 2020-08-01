#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class MoogVCF : public UnaryOpNode
{
public:
    MoogVCF(NodeRef input = 0.0, NodeRef cutoff = 200.0, NodeRef resonance = 0.0);

    NodeRef cutoff;
    NodeRef resonance;

    virtual void process(sample **out, int num_frames);

private:
    float out1[SIGNALFLOW_MAX_CHANNELS],
        out2[SIGNALFLOW_MAX_CHANNELS],
        out3[SIGNALFLOW_MAX_CHANNELS],
        out4[SIGNALFLOW_MAX_CHANNELS];
    float in1[SIGNALFLOW_MAX_CHANNELS],
        in2[SIGNALFLOW_MAX_CHANNELS],
        in3[SIGNALFLOW_MAX_CHANNELS],
        in4[SIGNALFLOW_MAX_CHANNELS];
};

REGISTER(MoogVCF, "moog")
}
