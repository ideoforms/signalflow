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

        template<class T>
        class SynthRefT : public std::shared_ptr<T>
        {
                public:

                        SynthRefT() : std::shared_ptr<Synth>(nullptr) { }
                        SynthRefT(Synth *ptr) : std::shared_ptr<Synth>(ptr) { }
                        SynthRefT(StructRef structure) : std::shared_ptr<Node>(new Synth(structure)) { }
        };

        typedef SynthRefT <Synth> SynthRef;
}
