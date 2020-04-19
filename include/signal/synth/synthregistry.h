#pragma once

#include <unordered_map>
#include "signal/synth/synthspec.h"

namespace libsignal
{
    class SynthRegistry
    {
        public:
            SynthRegistry();
            static SynthRegistry *global();

            Synth *create(std::string name);
            void add(std::string name, SynthSpecRef synthspec);
            SynthSpecRef get(std::string name);

            std::unordered_map <std::string, SynthSpecRef> synthspecs;
    };
}
