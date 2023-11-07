#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Attack-sustain-release envelope.
 *---------------------------------------------------------------------------------*/
class ASREnvelope : public Node
{
public:
    ASREnvelope(NodeRef attack = 0.1, NodeRef sustain = 0.5, NodeRef release = 0.1, NodeRef curve = 1.0, NodeRef clock = nullptr);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef attack;
    NodeRef sustain;
    NodeRef release;
    NodeRef curve;
    NodeRef clock;

    std::vector<float> phase;
};

REGISTER(ASREnvelope, "asr-envelope")

}
