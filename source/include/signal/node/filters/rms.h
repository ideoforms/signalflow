#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class RMS : public UnaryOpNode
{
public:
    RMS(NodeRef input = 0.0);

    virtual void process(sample **out, int num_frames);
};

REGISTER(RMS, "rms")

}
