#pragma once

#include "signal/core/graph.h"
#include "signal/node/node.h"

#include <string>
#include <iostream>
#include <sstream>

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
            std::set <Node *> rendered_nodes;
    };
}
