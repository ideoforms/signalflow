#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class EnvelopeRect : public Node
{
public:
    EnvelopeRect(NodeRef sustain = 1.0, NodeRef clock = nullptr);

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef sustain;
    NodeRef clock;

    std::vector<float> phase;
};

REGISTER(EnvelopeRect, "envelope-rect")

}
