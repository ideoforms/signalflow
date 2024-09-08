#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Offsets the input by a specified number of channels. With an N-channel input
 * and an offset of M, the output will have M+N channels.
 *---------------------------------------------------------------------------------*/
class ChannelOffset : public UnaryOpNode
{

public:
    ChannelOffset(int offset = 0, NodeRef input = nullptr);

    virtual void process(Buffer &out, int num_frames);

private:
    PropertyRef offset;
};

REGISTER(ChannelOffset, "channel-offset")

}
