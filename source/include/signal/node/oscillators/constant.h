#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class Constant : public Node
{
public:
    Constant(sample value = 0);

    float value;

    virtual void process(sample **out, int num_frames);
};

REGISTER(Constant, "constant")
}
