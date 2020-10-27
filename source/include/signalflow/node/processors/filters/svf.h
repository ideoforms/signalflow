#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class SVFFilter : public UnaryOpNode
{
public:
    SVFFilter(NodeRef input = 0.0,
              signalflow_filter_type_t filter_type = SIGNALFLOW_FILTER_TYPE_LOW_PASS,
              NodeRef cutoff = 440,
              NodeRef resonance = 0.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    signalflow_filter_type_t filter_type;
    NodeRef cutoff;
    NodeRef resonance;

    virtual void _recalculate(int frame);

    std::vector<float> ic1eq, ic2eq, g, k, a1, a2, a3;
};

REGISTER(SVFFilter, "svf-filter")

}
