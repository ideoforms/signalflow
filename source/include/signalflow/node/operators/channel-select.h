#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Select a subset of channels from a multichannel input, starting at offset,
 * up to a maximum of maximum, with the given step.
 *---------------------------------------------------------------------------------*/
class ChannelSelect : public UnaryOpNode
{

public:
    ChannelSelect(NodeRef input = nullptr, int offset = 0, int maximum = 0, int step = 1);

    virtual void process(Buffer &out, int num_frames);

private:
    PropertyRef offset;
    PropertyRef maximum;
    PropertyRef step;
};

REGISTER(ChannelSelect, "channel-select")

}
