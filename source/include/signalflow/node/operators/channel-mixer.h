#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class ChannelMixer : public UnaryOpNode
{

public:
    ChannelMixer(int channels = 1, NodeRef input = 0);

    virtual void process(Buffer &out, int num_frames);
    virtual void update_channels();

    std::list<NodeRef> inputs;
    int channels;
    float amp_compensation;
};

REGISTER(ChannelMixer, "channel-mixer")

}
