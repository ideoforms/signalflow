#pragma once

#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

#include <iostream>
#include <sstream>
#include <string>

namespace signalflow
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
