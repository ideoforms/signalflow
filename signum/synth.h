#include "nodedef.h"

#include "gen/constant.h"
#include <iostream>
#include <memory>


namespace signum
{
	class Synth
	{
		public:
			Synth(SynthDef *def)
			{
				NodeDefinition nodedef = def->read_structure();
				this->output = this->instantiate(&nodedef);
			}

			UnitRef instantiate(NodeDefinition *node)
			{
				/*------------------------------------------------------------------------
				 * Recursively instantiate the subgraph specified in NodeDefinition.
				 * Does not currently support graphs that route one node to multiple
				 * inputs.
				 *-----------------------------------------------------------------------*/
				NodeRegistry *registry = NodeRegistry::global();

				Unit *unit = registry->create(node->name);
				UnitRef unitref = UnitRef(unit);

				for (auto param : node->params)
				{
					std::string param_name = param.first;
					UnitRef param_unit = this->instantiate(param.second);
					unitref->set_param(param_name, param_unit);
				}

				if (node->is_constant)
				{
					// TODO rewrite
					Constant *constant = (Constant *) unit;
					constant->value = node->value;
				}

				if (!node->input_name.empty())
				{
					this->inputs[node->input_name] = unitref;
				}

				return unitref;
			}

			void set_param(std::string name, float value)
			{
				// TODO: Should support non-constant inputs
				UnitRef input = this->inputs[name];
				Constant *constant = (Constant *) input.get();
				constant->value = value;
			}

			UnitRef output;
			std::unordered_map <std::string, UnitRef> inputs;
	};

	template<class T>
	class SynthRefT : public std::shared_ptr<T>
	{
		public:
			using std::shared_ptr<T>::shared_ptr;

			SynthRefT() : std::shared_ptr<Synth>(nullptr) { }
			SynthRefT(Synth *ptr) : std::shared_ptr<Synth>(ptr) { }
			SynthRefT(SynthDef *def) : std::shared_ptr<Unit>(new Synth(def)) { }
	};

	typedef SynthRefT <Synth> SynthRef;
}
