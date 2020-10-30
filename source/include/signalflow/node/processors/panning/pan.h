#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class LinearPanner : public Node
{
public:
    LinearPanner(int channels = 2, NodeRef input = 0, NodeRef pan = 0.0);

    virtual void process(Buffer &out, int num_frames) override;

    NodeRef input;
    NodeRef pan;
};

REGISTER(LinearPanner, "linear-panner")
}
