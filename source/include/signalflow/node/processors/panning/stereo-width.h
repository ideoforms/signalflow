#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Reduces the width of a stereo signal.
 * When `width` = 1, input is unchanged.
 * When `width` = 0, outputs a pair of identical channels both containing L+R.
 *---------------------------------------------------------------------------------*/
class StereoWidth : public UnaryOpNode
{
public:
    StereoWidth(NodeRef input = 0, NodeRef width = 1);
    virtual void process(Buffer &out, int num_frames);

    NodeRef width;
};

REGISTER(StereoWidth, "stereo-width")
}
