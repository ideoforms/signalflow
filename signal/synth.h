#pragma once

#include "nodedef.h"
#include "structure.h"

namespace libsignal
{
	class Synth
	{
		public:
			Synth(StructRef structure);
			Synth(std::string name);
			NodeRef instantiate(NodeDefinition *nodedef);

			void set_param(std::string name, float value);
			void set_param(std::string name, NodeRef value);

			NodeRef output;
			std::unordered_map <std::string, NodeRef> inputs;
			std::set <NodeRef> nodes;
        };

        class SynthRef : public std::shared_ptr<Synth>
        {
                public:

                        SynthRef() : std::shared_ptr<Synth>(nullptr) { }
                        SynthRef(Synth *ptr) : std::shared_ptr<Synth>(ptr) { }
                        SynthRef(StructRef structure) : std::shared_ptr<Synth>(new Synth(structure)) { }
        };
}
