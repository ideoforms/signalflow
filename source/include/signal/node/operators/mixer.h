#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

#include <list>

namespace libsignal
{

class Mixer : public UnaryOpNode
{

public:
    Mixer(int channels = 1, NodeRef input = 0);

    virtual void process(sample **out, int num_frames);
    virtual void update_channels();

    std::list<NodeRef> inputs;
    int channels;
    float amp_compensation;
};

REGISTER(Mixer, "mixer")

}
