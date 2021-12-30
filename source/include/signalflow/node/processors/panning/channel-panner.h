#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class ChannelPanner : public Node
{
public:
    ChannelPanner(int channels = 2, NodeRef input = 0, NodeRef pan = 0.0, NodeRef width = 1.0);

    virtual void process(Buffer &out, int num_frames) override;

    NodeRef input;
    NodeRef pan;
    NodeRef width;
};

REGISTER(ChannelPanner, "channel-panner")
}
