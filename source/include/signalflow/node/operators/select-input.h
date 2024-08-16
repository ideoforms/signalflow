#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Pass through the output of one or more `inputs`, based on the integer input index
 * specified in `index`. Unlike `ChannelSelect`, inputs may be multichannel,
 * and `index` can be modulated in real time.
 *---------------------------------------------------------------------------------*/
class SelectInput : public VariableInputNode
{

public:
    SelectInput(NodeRef index = 0);
    SelectInput(std::initializer_list<NodeRef> inputs, NodeRef index = 0);
    SelectInput(std::vector<NodeRef> inputs, NodeRef index = 0);
    SelectInput(std::vector<int> inputs, NodeRef index = 0);
    SelectInput(std::vector<float> inputs, NodeRef index = 0);

    virtual void process(Buffer &out, int num_frames);

protected:
    void init(NodeRef index);
    NodeRef index;
};

REGISTER(SelectInput, "select-input")

}
