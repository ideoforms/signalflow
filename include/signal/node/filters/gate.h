#pragma once

#include "signal/node/node.h"
#include "signal/core/constants.h"

namespace libsignal
{
    class Gate : public UnaryOpNode
    {
        public:
            Gate(NodeRef input = 0.0, NodeRef threshold = 0.1);

            virtual void process(sample **out, int num_frames);

            NodeRef threshold;

        private:
            float level;

    };

    REGISTER(Gate, "gate");
}
