#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Takes an array of inputs and spreads them across multiple channels of output.
 *---------------------------------------------------------------------------------*/
class ChannelArray : public VariableInputNode
{

public:
    ChannelArray();
    ChannelArray(std::initializer_list<NodeRef> inputs);
    ChannelArray(std::vector<NodeRef> inputs);
    ChannelArray(std::vector<int> inputs);
    ChannelArray(std::vector<float> inputs);

    virtual void process(Buffer &out, int num_frames);
    virtual void update_channels();

protected:
    void init();
};

REGISTER(ChannelArray, "channel-array")

}
