#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class StereoBalance : public UnaryOpNode
{
public:
    StereoBalance(NodeRef input = 0, NodeRef balance = 0);
    virtual void process(Buffer &out, int num_frames);

    NodeRef balance;
};

REGISTER(StereoBalance, "stereo-balance")
}
