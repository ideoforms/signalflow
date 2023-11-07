#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Attack-decay-sustain-release envelope.
 * Sustain portion is held until gate is zero.
 *---------------------------------------------------------------------------------*/
class ADSREnvelope : public Node
{
public:
    ADSREnvelope(NodeRef attack = 0.1, NodeRef decay = 0.1, NodeRef sustain = 0.5,
                 NodeRef release = 0.1, NodeRef gate = 0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

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

REGISTER(ADSREnvelope, "adsr-envelope")

}
