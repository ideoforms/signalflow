#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class LinearPanner : public Node
{
public:
    LinearPanner(int channels = 2, NodeRef input = 2, NodeRef pan = 0.5);

    virtual void process(sample **out, int num_frames);

    NodeRef input;
    NodeRef pan;
};

REGISTER(LinearPanner, "linear-panner")
}
