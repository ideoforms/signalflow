#include "synth.h"

#include "oscillators/constant.h"
#include "synthregistry.h"
#include "core.h"

#include <iostream>
#include <memory>

namespace libsignal
{

Synth::Synth(SynthSpecRef synthspec)
{
	NodeDefinition nodedef = synthspec->get_root();
	this->output = this->instantiate(&nodedef);
}

Synth::Synth(SynthTemplateRef synthtemplate) : Synth(synthtemplate->parse())
{
}

Synth::Synth(std::string name)
{
	SynthSpecRef synthspec = SynthRegistry::global()->get(name);
	if (synthspec)
	{
		NodeDefinition nodedef = synthspec->get_root();
		this->output = this->instantiate(&nodedef);
	}
}

NodeRef Synth::instantiate(NodeDefinition *nodedef)
{
	/*------------------------------------------------------------------------
	 * Recursively instantiate the subgraph specified in NodeDefinition.
	 * Does not currently support graphs that route one node to multiple
	 * inputs.
	 *-----------------------------------------------------------------------*/
	NodeRegistry *registry = NodeRegistry::global();

	Node *node = registry->create(nodedef->name);
	NodeRef noderef = NodeRef(node);

	/*------------------------------------------------------------------------
	 * Update the synth's internal collection of node refs.
	 *-----------------------------------------------------------------------*/
	this->nodes.insert(noderef);

	for (auto param : nodedef->params)
	{
		std::string param_name = param.first;
		NodeRef param_node = this->instantiate(param.second);
		noderef->set_input(param_name, param_node);
	}

	if (nodedef->is_constant)
	{
		Constant *constant = (Constant *) node;
		constant->value = nodedef->value;
	}

	if (!nodedef->input_name.empty())
	{
		this->inputs[nodedef->input_name] = noderef;
	}

	return noderef;
}

void Synth::set_input(std::string name, float value)
{
	NodeRef current = this->inputs[name];
	signal_assert(this->inputs[name] != nullptr, "Synth has no such parameter: %s", name.c_str());
	NodeRef input = this->inputs[name];
	Constant *constant = (Constant *) input.get();
	constant->value = value;
}

void Synth::set_input(std::string name, NodeRef value)
{
	/*------------------------------------------------------------------------
	 * Replace a named input with another node.
	 * Iterate over this synth's nodes, replacing the prior input with
	 * the new node. (Inefficient, should be rethought.)
	 *-----------------------------------------------------------------------*/
	signal_assert(this->inputs[name] != nullptr, "Synth has no such parameter: %s", name.c_str());
	NodeRef current = this->inputs[name];
	for (NodeRef node : this->nodes)
	{
		for (auto param : node->params)
		{
			if ((param.second)->get() == current.get())
			{
				// Update routing
				// printf("Updating '%s' input of %s\n", param.first.c_str(), node->name.c_str());
				node->set_input(param.first, value);
			}
		}
	}
	this->inputs[name] = value;
}

void Synth::disconnect()
{
	this->output->disconnect_outputs();
	for (auto input : this->inputs)
	{
		std::string name = input.first;
		this->set_input(name, 0);
	}
}

}
