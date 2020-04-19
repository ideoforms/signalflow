#pragma once

#include "signal/node/node.h"
#include "signal/core/constants.h"

namespace libsignal
{
    class Freeze : public UnaryOpNode
    {
        public:
            Freeze(NodeRef input = 0);

            Buffer *buffer;
            bool frozen;
            bool trig;

            virtual void process(sample **out, int num_frames);
            virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);
    };

    REGISTER(Freeze, "freeze");
}
