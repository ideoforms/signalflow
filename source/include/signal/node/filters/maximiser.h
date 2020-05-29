#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class Maximiser : public UnaryOpNode
{
public:
    Maximiser(NodeRef input = 0.0,
              NodeRef ceiling = 0.5,
              NodeRef attack_time = 1.0,
              NodeRef release_time = 1.0);

    virtual void process(sample **out, int num_frames);

    NodeRef ceiling;
    NodeRef attack_time;
    NodeRef release_time;

private:
    float gain;
};

REGISTER(Maximiser, "maximiser")
}
