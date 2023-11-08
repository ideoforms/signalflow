#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Map a linear amplitude value to decibels.
 *---------------------------------------------------------------------------------*/
class AmplitudeToDecibels : public UnaryOpNode
{
public:
    AmplitudeToDecibels(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

class DecibelsToAmplitude : public UnaryOpNode
{
public:
    DecibelsToAmplitude(NodeRef a = 0);
    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(AmplitudeToDecibels, "amplitude-to-decibels")
REGISTER(DecibelsToAmplitude, "decibels-to-amplitude")

}
