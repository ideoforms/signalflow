#pragma once

#include "../node.h"

namespace libsignal
{
    class Width : public UnaryOpNode
    {
        public:
            Width(NodeRef input = 0, NodeRef width = 1);
            virtual void process(sample **out, int num_frames);

            NodeRef width;

    };

    REGISTER(Width, "width");
}
