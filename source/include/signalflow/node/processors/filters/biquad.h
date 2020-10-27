#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class BiquadFilter : public UnaryOpNode
{
public:
    BiquadFilter(NodeRef input = 0.0,
                 signalflow_filter_type_t filter_type = SIGNALFLOW_FILTER_TYPE_LOW_PASS,
                 NodeRef cutoff = 440,
                 NodeRef resonance = 0.0,
                 NodeRef peak_gain = 0.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    signalflow_filter_type_t filter_type;
    NodeRef cutoff;
    NodeRef resonance;
    NodeRef peak_gain;

    virtual void _recalculate();

    std::vector<float> a0, a1, a2, b1, b2, z1, z2;
};

REGISTER(BiquadFilter, "biquad-filter")

}
