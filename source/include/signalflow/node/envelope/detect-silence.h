#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Detects blocks of silence below the threshold value. Used as an auto-free
 * node to terminate a Patch after processing is complete.
 *---------------------------------------------------------------------------------*/
class DetectSilence : public UnaryOpNode
{
public:
    DetectSilence(NodeRef input = nullptr, NodeRef threshold = 0.00001);
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef threshold;
};

REGISTER(DetectSilence, "detect-silence")

}
