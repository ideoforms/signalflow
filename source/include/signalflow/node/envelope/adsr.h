#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class EnvelopeADSR : public Node
{
public:
    EnvelopeADSR(NodeRef attack = 0.1, NodeRef decay = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef gate = 0);

    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef attack;
    NodeRef decay;
    NodeRef sustain;
    NodeRef release;
    NodeRef gate;

    float phase;
    float level;
    signalflow_curve_t curve;
    bool released = false;
    float release_decrement_per_frame;
};

REGISTER(EnvelopeADSR, "envelope-adsr")

}
