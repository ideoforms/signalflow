#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class EnvelopeASR : public Node
{
public:
    EnvelopeASR(NodeRef attack = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef clock = nullptr);

    NodeRef attack;
    NodeRef sustain;
    NodeRef release;
    NodeRef clock = nullptr;

    std::vector<float> phase;
    signalflow_curve_t curve;

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(EnvelopeASR, "envelope-asr")

}
