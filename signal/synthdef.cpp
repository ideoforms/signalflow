#include "synthdef.h"

#include "core.h"
#include "gen/constant.h"
#include "json11/json11.hpp"

#include <iostream>
#include <fstream>

using namespace json11;

namespace libsignal
{
	SynthDef::SynthDef(std::string name)
	{
		this->name = name;
	}
	
	/*------------------------------------------------------------------------
	 * TEMPLATING 
	 *-----------------------------------------------------------------------*/

	NodeRef SynthDef::add_input(std::string name, sample default_value)
	{
		NodeRef placeholder(default_value);
		this->inputs[name] = placeholder.get();
		return placeholder;
	}

	std::string SynthDef::get_input_name(const NodeRef &node)
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

	NodeRef SynthDef::add_node(NodeRef node)
	{
		return node;
	} 
	void SynthDef::set_output(const NodeRef &out)
	{
		this->output = out;
	}

	void SynthDef::save(std::string filename)
	{
	}

	void SynthDef::load(std::string filename)
	{
		std::string buf;
		std::string line;
		std::string err;

		std::ifstream input(filename);
		while (std::getline(input, line))
		{
			buf += line + "\n";
		}

		auto json = Json::parse(buf, err);
		if (!err.empty())
		{
			printf("Failed: %s\n", err.c_str());
		}

		if (!json.is_array())
		{
			printf("Must be array\n");
		}
		for (auto element : json.array_items())
		{
			NodeDefinition node;
			bool is_output = false;
			for (auto pair : element.object_items())
			{
				std::string key = pair.first;
				auto value = pair.second;

				printf("Got key %s\n", key.c_str());
				if (key == "node")
				{
					printf("Set name\n");
					node.set_name(value.string_value());
				}
				else if (key == "id")
				{
					node.set_id(value.int_value());
				}
				else if (key == "is_output")
				{
					is_output = true;
				}
				else
				{
					if (value.is_number())
					{
						node.add_param(key, value.number_value());
					}
					else if (value.is_object())
					{
						int id = value["id"].int_value();
						NodeDefinition *ptr = this->get_node_def(id);
						node.add_param(key, ptr);
					}
				}
			}
			printf("Adding node with name %s\n", node.name.c_str());
			this->add_node_def(node);
			if (is_output)
			{
				this->set_output(node);
			}
		}
	}

	void SynthDef::add_node_def(NodeDefinition def)
	{
		this->nodedefs[def.id] = def;
	} 

	NodeDefinition *SynthDef::get_node_def(int id)
	{
		return &(this->nodedefs[id]);
	} 

	void SynthDef::set_output(NodeDefinition def)
	{
		this->output_def = def;
	} 

	/*------------------------------------------------------------------------
	 * Scans the graph structure beginning from its inputs and outputs.
	 *-----------------------------------------------------------------------*/
	void SynthDef::read_structure()
	{
		signal_assert(this->output != nullptr, "SynthDef %s: output is not set", this->name.c_str());
		const NodeRef &r = this->output;
		this->last_id = 0;
		this->output_def = this->read_structure(r);
		this->parsed = true;
	}

	NodeDefinition SynthDef::read_structure(const NodeRef &node)
	{
		NodeDefinition def(node->name);
		def.set_id(this->last_id++);

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

	NodeDefinition SynthDef::get_root()
	{
		return this->output_def;
	}

}
