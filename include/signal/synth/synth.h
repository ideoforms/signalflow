#pragma once

#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"
#include "signal/synth/synthtemplate.h"

namespace libsignal
{
    class Synth
    {
        public:
            Synth(SynthSpecRef synthspec);
            Synth(SynthTemplateRef synthtemplate);
            Synth(std::string name);
            NodeRef instantiate(NodeDefinition *nodedef);

            void set_input(std::string name, float value);
            void set_input(std::string name, NodeRef value);
            void disconnect();

            NodeRef output;
            std::unordered_map <std::string, NodeRef> inputs;
            std::set <NodeRef> nodes;
    };

    class SynthRef : public std::shared_ptr<Synth>
    {
        public:

            SynthRef() : std::shared_ptr<Synth>(nullptr) { }
            SynthRef(Synth *ptr) : std::shared_ptr<Synth>(ptr) { }
            SynthRef(SynthSpecRef synthspec) : std::shared_ptr<Synth>(new Synth(synthspec)) { }
    };
}
