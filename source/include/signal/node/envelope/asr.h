#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{

class EnvelopeASR : public Node
{
public:
    EnvelopeASR(NodeRef attack = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef clock = nullptr);

    NodeRef attack;
    NodeRef sustain;
    NodeRef release;
    NodeRef clock = nullptr;

    float phase;

    virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);
    virtual void process(sample **out, int num_frames);
};

REGISTER(EnvelopeASR, "envelope-asr")

}
