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

			UnitRef add_node(UnitRef node)
			{
				return node;
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
					Constant *constant = (Constant *) unit.get();
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

}
