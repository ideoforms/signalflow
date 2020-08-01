#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class EnvelopeADSR : public Node
{
public:
    EnvelopeADSR(NodeRef attack = 0.1, NodeRef decay = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef gate = 0);

    NodeRef attack;
    NodeRef decay;
    NodeRef sustain;
    NodeRef release;
    NodeRef gate;

    virtual void process(sample **out, int num_frames);

private:
    float phase;
    signalflow_curve_t curve;
    bool released = false;
};

REGISTER(EnvelopeADSR, "envelope-adsr")

}
