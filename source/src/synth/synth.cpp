#include "signal/synth/synth.h"

#include "signal/core/core.h"
#include "signal/core/graph.h"
#include "signal/node/oscillators/constant.h"
#include "signal/synth/synthregistry.h"

#include <iostream>
#include <memory>

namespace libsignal
{

extern AudioGraph *shared_graph;

Synth::Synth()
{
    this->graph = shared_graph;
    this->auto_free = false;
}

Synth::Synth(SynthSpecRef synthspec)
    : Synth()
{
    NodeDefinition nodedef = synthspec->get_root();
    this->output = this->instantiate(&nodedef);
}

Synth::Synth(std::string name)
    : Synth()
{
    SynthSpecRef synthspec = SynthRegistry::global()->get(name);
    if (synthspec)
    {
        NodeDefinition nodedef = synthspec->get_root();
        this->output = this->instantiate(&nodedef);
    }
}

Synth::~Synth()
{
}

signal_synth_state_t Synth::get_state()
{
    return this->state;
}

void Synth::set_state(signal_synth_state_t state)
{
    this->state = state;
}

NodeRef Synth::instantiate(NodeDefinition *nodedef)
{
    /*------------------------------------------------------------------------
     * Recursively instantiate the subgraph specified in NodeDefinition.
     * Does not currently support graphs that route one node to multiple
     * inputs.
     *-----------------------------------------------------------------------*/
    NodeRegistry *registry = NodeRegistry::global();

    NodeRef noderef;

    if (!nodedef->input_name.empty() && this->inputs[nodedef->input_name])
    {
        noderef = this->inputs[nodedef->input_name];
    }
    else
    {
        Node *node = registry->create(nodedef->name);
        noderef = NodeRef(node);

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

        noderef->set_synth(this);
    }

    return noderef;
}

void Synth::set_input(std::string name, NodeRef value)
{
    /*------------------------------------------------------------------------
     * Replace a named input with another node.
     * Iterate over this synth's nodes, replacing the prior input with
     * the new node. (Inefficient, should be rethought.)
     *-----------------------------------------------------------------------*/
    if (this->inputs[name] == nullptr)
    {
        throw std::runtime_error("Synth has no such parameter: " + name);
    }
    NodeRef current = this->inputs[name];
    for (NodeRef node : this->nodes)
    {
        for (auto param : node->inputs)
        {
            if ((param.second)->get() == current.get())
            {
                // Update routing
                node->set_input(param.first, value);
            }
        }
    }
    this->inputs[name] = value;
}

void Synth::disconnect()
{
    this->graph->remove_output(this);
}

bool Synth::get_auto_free()
{
    return this->auto_free;
}
void Synth::set_auto_free(bool value)
{
    this->auto_free = value;
}

void Synth::node_state_changed(Node *node)
{
    if (node->get_state() == SIGNAL_NODE_STATE_FINISHED && this->auto_free)
    {
        this->set_state(SIGNAL_SYNTH_STATE_FINISHED);
        this->disconnect();
    }
}

/*------------------------------------------------------------------------
 * TEMPLATING
 *-----------------------------------------------------------------------*/

NodeRef Synth::add_input(std::string name, sample default_value)
{
    NodeRef placeholder(default_value);
    this->inputs[name] = placeholder;
    nodes.insert(placeholder);
    return placeholder;
}

std::string Synth::_get_input_name(const NodeRef &node)
{
    for (auto input : this->inputs)
    {
        std::string name = input.first;
        NodeRef node_ptr = input.second;
        if (node_ptr.get() == node.get())
            return name;
    }

    return "";
}

NodeRef Synth::add_node(NodeRef node)
{
    nodes.insert(node);
    return node;
}

void Synth::set_output(NodeRef out)
{
    // check if out is in synth
    this->output = out;
}

/*------------------------------------------------------------------------
 * Scans the synth graph beginning from its outputs.
 *-----------------------------------------------------------------------*/
SynthSpecRef Synth::parse()
{
    // TODO: Currently have parsed property in this object and spec
    if (this->output == nullptr)
    {
        throw std::runtime_error("Synth " + this->name + ": output is not set");
    }
    NodeRef root = this->output;
    this->last_id = 0;

    SynthSpecRef spec = new SynthSpec(this->name);
    spec->output_def = this->_parse_from_node(root);
    spec->parsed = true;
    spec->nodedefs = this->nodedefs;
    for (auto node : nodes)
    {
        if (parsed_nodes.find(node) == parsed_nodes.end())
        {
            throw std::runtime_error("Synth contains unconnected node (" + node->name + ").");
        }
    }

    return spec;
}

NodeDefinition Synth::_parse_from_node(const NodeRef &node)
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
        for (auto param : node->inputs)
        {
            NodeRef param_node = *(param.second);
            if (param_node)
            {
                NodeDefinition param_def = this->_parse_from_node(param_node);
                def.add_input(param.first, &param_def);
            }
        }
    }

    std::string input_name = this->_get_input_name(node);
    if (!input_name.empty())
    {
        def.input_name = input_name;
    }

    this->nodedefs[def.id] = def;
    this->parsed_nodes.insert(node);

    return def;
}

}
