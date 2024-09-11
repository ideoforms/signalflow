#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Bus is a node with a fixed number of input channels and arbitrary number of
 * inputs, used to aggregate multiple sources. It is similar to Sum, but with
 * a defined channel count that does not adapt to its inputs.
 *---------------------------------------------------------------------------------*/
class Bus : public VariableInputNode
{

public:
    Bus(unsigned int num_channels = 1);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Bus, "bus")

}
