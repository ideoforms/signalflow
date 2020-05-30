#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class Smooth : public UnaryOpNode
{
public:
    Smooth(NodeRef input = nullptr, NodeRef smooth = 0.99);

    virtual void process(sample **out, int num_frames);

private:
    NodeRef smooth;
    std::vector <sample> values;

};

REGISTER(Smooth, "smooth")
}
