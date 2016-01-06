#pragma once

#include <string>
#include <unordered_map>

namespace signum
{

class NodeDefinition
{
	public:
		NodeDefinition() : name("(null)")
		{
		}

		~NodeDefinition()
		{
			// free params
		}

		NodeDefinition(std::string name) : name(name)
		{
		}

		NodeDefinition(std::string name, float value) : name(name)
		{
			this->set_value(value);
		}

		void add_param(std::string name, NodeDefinition *def)
		{
			NodeDefinition *def_copy = new NodeDefinition();
			*def_copy = *def;
			this->params[name] = def_copy;
		}

		void add_param(std::string name, float value)
		{
			this->params[name] = new NodeDefinition("constant", value);
		}

		void set_value (float value)
		{
			this->value = value;
			this->is_constant = true;
		}

		std::string name;
		float value;
		bool is_constant = false;
		std::unordered_map <std::string, NodeDefinition *> params;

		std::string input_name;
};

}
