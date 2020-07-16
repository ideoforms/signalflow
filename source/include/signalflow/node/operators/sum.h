#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <list>

namespace signalflow
{

class Sum : public Node
{

public:
    Sum();
    Sum(std::initializer_list<NodeRef> inputs);
    Sum(std::vector<NodeRef> inputs);
    Sum(std::vector<int> inputs);
    Sum(std::vector<float> inputs);

    virtual void process(sample **out, int num_frames);

    using Node::add_input;
    virtual void add_input(NodeRef input);
    virtual void set_input(std::string name, const NodeRef &node);

    std::list<NodeRef> input_list;
};

REGISTER(Sum, "sum")

}
