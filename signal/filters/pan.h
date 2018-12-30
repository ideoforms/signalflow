#pragma once

#include "../node.h"

namespace libsignal
{
    class Pan : public Node
    {
        public:
            Pan(int channels = 2, NodeRef input = 2, NodeRef pan = 0.5);

            virtual void process(sample **out, int num_frames);

            NodeRef input;
            NodeRef pan;

    };

    REGISTER(Pan, "pan");
}
