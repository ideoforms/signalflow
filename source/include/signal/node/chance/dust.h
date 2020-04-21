#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class Dust : public Node
{
public:
    Dust(NodeRef frequency = 1.0);

    NodeRef frequency;

    virtual void process(sample **out, int num_frames);

private:
    int steps_remaining;
};

REGISTER(Dust, "dust");
}
