#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Buss the output of all of the input nodes, by sample.
 *---------------------------------------------------------------------------------*/
class Bus : public VariableInputNode
{

public:
    Bus(unsigned int num_channels = 1);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(Bus, "bus")

}
