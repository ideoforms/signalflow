#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class MoogVCF : public UnaryOpNode
{
public:
    MoogVCF(NodeRef input = 0.0, NodeRef cutoff = 200.0, NodeRef resonance = 0.0);

    NodeRef cutoff;
    NodeRef resonance;

    virtual void process(sample **out, int num_frames);

private:
    float out1[SIGNAL_MAX_CHANNELS],
        out2[SIGNAL_MAX_CHANNELS],
        out3[SIGNAL_MAX_CHANNELS],
        out4[SIGNAL_MAX_CHANNELS];
    float in1[SIGNAL_MAX_CHANNELS],
        in2[SIGNAL_MAX_CHANNELS],
        in3[SIGNAL_MAX_CHANNELS],
        in4[SIGNAL_MAX_CHANNELS];
};

REGISTER(MoogVCF, "moog");
}
