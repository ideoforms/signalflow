#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Calculates the zero-crossing rate of the input signal.
 * Outputs the rate at which the signal crosses zero.
 * Useful for simple pitch estimation.
 *---------------------------------------------------------------------------------*/
class ZeroCrossingRate : public UnaryOpNode
{
public:
    ZeroCrossingRate(NodeRef input = nullptr);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(ZeroCrossingRate, "zero-crossing-rate")
}
