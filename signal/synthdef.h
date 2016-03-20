#include "nodedef.h"

#include "gen/constant.h"
#include <iostream>

namespace libsignal
{
	class SynthDef
	{
		public:
			SynthDef(std::string name)
			{
				this->name = name;
			}

			NodeRef add_input(std::string name, sample default_value)
			{
				NodeRef placeholder(default_value);
				this->inputs[name] = placeholder.get();
				return placeholder;
			}

			std::string get_input_name(const NodeRef &node)
			{
				for (auto input : this->inputs)
				{
					std::string name = input.first;
					Node *node_ptr = input.second;
					if (node_ptr == node.get())
						return name;
				}

				return "";
			}

			NodeRef add_node(NodeRef node)
			{
				return node;
			}

			void set_output(const NodeRef &out)
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
				signal_assert(this->output != nullptr, "SynthDef %s: output is not set", this->name.c_str());
				const NodeRef &r = this->output;
				return this->read_structure(r);
			}

			NodeDefinition read_structure(const NodeRef &node)
			{
				NodeDefinition def(node->name);
				if (node->name == "constant")
				{
					Constant *constant = (Constant *) node.get();
					def.set_value(constant->value);
				}
				else
				{
					for (auto param : node->params)
					{
						NodeRef param_node = *(param.second);
						if (param_node)
						{
							NodeDefinition param_def = this->read_structure(param_node);
							def.add_param(param.first, &param_def);
						}
					}
				}

				std::string input_name = this->get_input_name(node);
				if (!input_name.empty())
				{
					def.input_name = input_name;
				}

				return def;
			}

			std::string name;
			std::unordered_map <std::string, Node *> inputs;
			NodeRef output = nullptr;

			std::vector <NodeDefinition> nodedefs;
	};

}
