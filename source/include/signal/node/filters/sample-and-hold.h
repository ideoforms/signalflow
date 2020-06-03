#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class SampleAndHold : public UnaryOpNode
{
public:
    SampleAndHold(NodeRef input = nullptr, NodeRef clock = nullptr);

    virtual void process(sample **out, int num_frames);
    virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);

private:
    NodeRef clock;
    std::vector<sample> values;
};

REGISTER(SampleAndHold, "sample-and-hold")
}
