#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Sums the output of all of the input nodes, by sample.
 *---------------------------------------------------------------------------------*/
class Sum : public Node
{

public:
    Sum();
    Sum(std::initializer_list<NodeRef> inputs);
    Sum(std::vector<NodeRef> inputs);
    Sum(std::vector<int> inputs);
    Sum(std::vector<float> inputs);

    virtual void process(Buffer &out, int num_frames);

    virtual void add_input(NodeRef input);
    virtual void remove_input(NodeRef input);
    virtual void set_input(std::string name, const NodeRef &node);

protected:
    std::list<NodeRef> input_list;
    int input_index = 0;
};

REGISTER(Sum, "sum")

}
