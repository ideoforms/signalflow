#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Takes a stereo input and rebalances it, where 0 is unchanged, -1 is hard left,
 * and 1 is hard right.
 *---------------------------------------------------------------------------------*/
class StereoBalance : public UnaryOpNode
{
public:
    StereoBalance(NodeRef input = 0, NodeRef balance = 0);
    virtual void process(Buffer &out, int num_frames);

    NodeRef balance;
};

REGISTER(StereoBalance, "stereo-balance")
}
