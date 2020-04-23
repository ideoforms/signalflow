#pragma once

#include "signal/node/node.h"

namespace libsignal
{
class Sine : public Node
{
public:
    Sine(NodeRef frequency = 440);

    NodeRef frequency;
    float phase[SIGNAL_MAX_CHANNELS];

    virtual void process(sample **out, int num_frames);
};

REGISTER(Sine, "sine")
}
