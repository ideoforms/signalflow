#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class Sine : public Node
{
public:
    Sine(NodeRef frequency = 440);

    NodeRef frequency;
    float *phase;

    virtual void process(sample **out, int num_frames);
    void allocate_memory();
    void free_memory();
};

REGISTER(Sine, "sine")
}
