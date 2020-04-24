#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

#include <list>

namespace libsignal
{

class Multiplex : public Node
{

public:
    Multiplex();
    Multiplex(std::initializer_list<NodeRef> inputs);
    Multiplex(std::vector<NodeRef> inputs);
    Multiplex(std::vector<float> inputs);

    virtual void process(sample **out, int num_frames);
    virtual void update_channels();

    using Node::add_input;
    virtual void add_input(NodeRef input);
    virtual void set_input(std::string name, const NodeRef &node);

    std::list<NodeRef> inputs;
};

REGISTER(Multiplex, "multiplex")

}
