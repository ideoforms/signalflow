#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class RectangularEnvelope : public Node
{
public:
    RectangularEnvelope(NodeRef sustain_duration = 1.0, NodeRef clock = nullptr);

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef sustain_duration;
    NodeRef clock;

    std::vector<float> phase;
};

REGISTER(RectangularEnvelope, "rectangular-envelope")

}
