#include "nodedef.h"

#include "gen/constant.h"
#include <iostream>

namespace signum
{
	class SynthDef
	{
		public:
			UnitRef add_input(std::string name, sample default_value)
			{
				UnitRef placeholder(default_value);
				this->inputs[name] = placeholder.get();
				return placeholder;
			}

			std::string get_input_name(const UnitRef &unit)
			{
				for (auto input : this->inputs)
				{
					std::string name = input.first;
					Unit *unit_ptr = input.second;
					if (unit_ptr == unit.get())
						return name;
				}

				return "";
			}

			void set_output(const UnitRef &out)
			{
				this->output = out;
			}

			void save(std::string filename)
			{
			}

			/*------------------------------------------------------------------------
			 * Scans the graph structure beginning from its inputs and outputs.
			 *-----------------------------------------------------------------------*/
			NodeDefinition read_structure()
			{
				const UnitRef &r = this->output;
				return this->read_structure(r);
			}

			NodeDefinition read_structure(const UnitRef &unit)
			{
				NodeDefinition def(unit->name);
				if (unit->name == "constant")
				{
					gen::Constant *constant = (gen::Constant *) unit.get();
					def.set_value(constant->value);
				}
				else
				{
					for (auto param : unit->params)
					{
						UnitRef param_unit = *(param.second);
						if (param_unit)
						{
							NodeDefinition param_def = this->read_structure(param_unit);
							def.add_param(param.first, param_def);
						}
					}
				}

				std::string input_name = this->get_input_name(unit);
				if (!input_name.empty())
				{
					def.input_name = input_name;
				}

				return def;
			}

			std::unordered_map <std::string, Unit *> inputs;
			UnitRef output;

			std::vector <NodeDefinition> nodedefs;
	};

	class Synth
	{
		public:
			Synth(SynthDef *def)
			{
				NodeDefinition nodedef = def->read_structure();
				this->output = this->instantiate(nodedef);
			}

			UnitRef instantiate(NodeDefinition node)
			{
				/*------------------------------------------------------------------------
				 * Recursively instantiate the subgraph specified in NodeDefinition.
				 * Does not currently support graphs that route one node to multiple
				 * inputs.
				 *-----------------------------------------------------------------------*/
				NodeRegistry *registry = NodeRegistry::global();

				Unit *unit = registry->create(node.name);
				UnitRef unitref = UnitRef(unit);

				for (auto param : node.params)
				{
					std::string param_name = param.first;
					UnitRef param_unit = this->instantiate(param.second);
					unitref->set_param(param_name, param_unit);
				}

				if (node.is_constant)
				{
					// TODO rewrite
					gen::Constant *constant = (gen::Constant *) unit;
					constant->value = node.value;
				}

				if (!node.input_name.empty())
				{
					this->inputs[node.input_name] = unitref;
				}

				return unitref;
			}

			/*
			void route(const UnitRef &other)
			{
				std::cout << "Output is " << this->output << std::endl;
				// this->output->route(other);
				// other->add_input(this->output);
			}
			*/

			void set_param(std::string name, float value)
			{
				// TODO: Should support non-constant inputs
				UnitRef input = this->inputs[name];
				gen::Constant *constant = (gen::Constant *) input.get();
				constant->value = value;
			}

			UnitRef output;
			std::unordered_map <std::string, UnitRef> inputs;
	};
}
