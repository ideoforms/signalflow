#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class StereoWidth : public UnaryOpNode
{
public:
    StereoWidth(NodeRef input = 0, NodeRef width = 1);
    virtual void process(Buffer &out, int num_frames);

    NodeRef width;
};

REGISTER(StereoWidth, "stereo-width")
}
