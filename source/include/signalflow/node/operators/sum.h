#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Sums the output of all of the input nodes, by sample.
 *---------------------------------------------------------------------------------*/
class Sum : public VariableInputNode
{

public:
    Sum();
    Sum(std::initializer_list<NodeRef> inputs);
    Sum(std::vector<NodeRef> inputs);
    Sum(std::vector<int> inputs);
    Sum(std::vector<float> inputs);

    virtual void process(Buffer &out, int num_frames);

private:
    void init();
};

REGISTER(Sum, "sum")

}
