#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{

class RoundToScale : public UnaryOpNode
{

public:
    RoundToScale(NodeRef a);

    virtual void process(sample **out, int num_frames);
};

}
