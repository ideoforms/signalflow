#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Outputs the root-mean-squared value of the input, in buffers equal to the
 * graph's current buffer size.
 *---------------------------------------------------------------------------------*/
class RMS : public UnaryOpNode
{
public:
    RMS(NodeRef input = 0.0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(RMS, "rms")

}
