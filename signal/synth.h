#include "nodedef.h"

#include "gen/constant.h"
#include <iostream>
#include <memory>


namespace libsignal
{
	class Synth
	{
		public:
			Synth(StructRef structure)
			{
				NodeDefinition nodedef = structure->get_root();
				this->output = this->instantiate(&nodedef);
			}

			NodeRef instantiate(NodeDefinition *nodedef)
			{
				/*------------------------------------------------------------------------
				 * Recursively instantiate the subgraph specified in NodeDefinition.
				 * Does not currently support graphs that route one node to multiple
				 * inputs.
				 *-----------------------------------------------------------------------*/
				NodeRegistry *registry = NodeRegistry::global();

				Node *node = registry->create(nodedef->name);
				NodeRef noderef = NodeRef(node);

				for (auto param : nodedef->params)
				{
					std::string param_name = param.first;
					NodeRef param_node = this->instantiate(param.second);
					noderef->set_param(param_name, param_node);
				}

				if (nodedef->is_constant)
				{
					// TODO rewrite
					Constant *constant = (Constant *) node;
					constant->value = nodedef->value;
				}

				if (!nodedef->input_name.empty())
				{
					this->inputs[nodedef->input_name] = noderef;
				}

				return noderef;
			}

			void set_param(std::string name, float value)
			{
				// TODO: Should support non-constant inputs
				NodeRef input = this->inputs[name];
				Constant *constant = (Constant *) input.get();
				constant->value = value;
			}

			NodeRef output;
			std::unordered_map <std::string, NodeRef> inputs;
	};

	template<class T>
	class SynthRefT : public std::shared_ptr<T>
	{
		public:
			using std::shared_ptr<T>::shared_ptr;

			SynthRefT() : std::shared_ptr<Synth>(nullptr) { }
			SynthRefT(Synth *ptr) : std::shared_ptr<Synth>(ptr) { }
			SynthRefT(StructRef structure) : std::shared_ptr<Node>(new Synth(structure)) { }
	};

	typedef SynthRefT <Synth> SynthRef;
}
