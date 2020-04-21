#pragma once

#include "signal/core/graph.h"
#include "signal/node/node.h"

#include <iostream>
#include <sstream>
#include <string>

namespace libsignal
{

class GraphRenderer
{
public:
    GraphRenderer();
    void render(AudioGraphRef graph);
    void render_node(NodeRef node);

    std::stringstream nodestream;
    std::stringstream edgestream;
    std::set<Node *> rendered_nodes;
};

}
