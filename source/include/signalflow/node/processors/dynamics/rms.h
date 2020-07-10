#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class RMS : public UnaryOpNode
{
public:
    RMS(NodeRef input = 0.0);

    virtual void process(sample **out, int num_frames);
};

REGISTER(RMS, "rms")

}
