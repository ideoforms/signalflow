#pragma once

#include "signalflow/node/node.h"

namespace signalflow
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
