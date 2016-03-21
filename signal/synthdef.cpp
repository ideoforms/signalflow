#include "synthdef.h"

#include "core.h"
#include "gen/constant.h"

#include <iostream>

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
