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
				this->inputs[name] = &placeholder;
				return placeholder;
			}

			void set_output(const UnitRef &out)
			{
				std::cout << "set_output : " << out << std::endl;
				this->output = out;
				std::cout << "this->output: " << this->output << std::endl;
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
				std::cout << "read_structure() : " << this->output << std::endl;
				return this->read_structure(r);
			}

			NodeDefinition read_structure(const UnitRef &unit)
			{
				std::cout << "read_structure: " << unit << std::endl;
				NodeDefinition def(unit->name);
				if (unit->name == "constant")
				{
					gen::Constant *constant = (gen::Constant *) unit.get();
					def.set_value(constant->value);
				}
				else
				{
					int index = 0;
					for (UnitRef input : unit->inputs)
					{
						NodeDefinition input_def = this->read_structure(input);
						char name[] = "input0";
						name[5] = '0' + index++;
						def.add_param(name, input_def);
					}
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

				return def;
			}

			std::unordered_map <std::string, UnitRef *> inputs;
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
				NodeRegistry *registry = NodeRegistry::global();

				Unit *u = registry->create(node.name);
				UnitRef unit = UnitRef(u);
				printf("node: %s\n", node.name.c_str());
				for (auto param : node.params)
				{
					printf(" - param: %s\n", param.first.c_str());
					std::string param_name = param.first;
					UnitRef param_unit = this->instantiate(param.second);
					unit->set_param(param_name, param_unit);
				}

				if (node.is_constant)
				{
					gen::Constant *constant = (gen::Constant *) u;
					constant->value = node.value;
				}

				return unit;
			}

			void route(const UnitRef &other)
			{
				std::cout << "Output is " << this->output << std::endl;
				// this->output->route(other);
				other->add_input(this->output);
			}

			UnitRef output;
	};
}
