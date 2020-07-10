#pragma once

#include "signalflow/node/node.h"

namespace signalflow
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
