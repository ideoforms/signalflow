#include "nodedef.h"

#include "gen/constant.h"

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

			void set_output(UnitRef out)
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
				return this->read_structure(this->output);
			}

			NodeDefinition read_structure(UnitRef unit)
			{
				NodeDefinition def(unit->name);
				printf("making nodedef %s\n", unit->name.c_str());
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
				this->instantiate(nodedef);
			}

			UnitRef instantiate(NodeDefinition node)
			{
				NodeRegistry *registry = NodeRegistry::global();

				UnitRef unit = registry->create(node);
				printf("node: %s\n", node.name.c_str());
				for (auto param : node.params)
				{
					printf(" - param: %s\n", param.first.c_str());
					UnitRef param_unit = this->instantiate(param.second);
				}

				return unit;
			}

			void route(UnitRef other)
			{
			}
	};
}
