#include "signal/patch/patch.h"

#include "signal/core/core.h"
#include "signal/core/graph.h"
#include "signal/node/oscillators/constant.h"
#include "signal/patch/patchregistry.h"

#include <iostream>
#include <memory>

namespace libsignal
{

extern AudioGraph *shared_graph;

Patch::Patch()
{
    this->graph = shared_graph;
    this->auto_free = false;
}

Patch::Patch(PatchSpecRef patchspec)
    : Patch()
{
    NodeSpec nodespec = patchspec->get_root();
    this->output = this->instantiate(&nodespec);
}

Patch::Patch(std::string name)
    : Patch()
{
    PatchSpecRef patchspec = PatchRegistry::global()->get(name);
    if (patchspec)
    {
        NodeSpec nodespec = patchspec->get_root();
        this->output = this->instantiate(&nodespec);
    }
}

Patch::~Patch()
{
}

signal_patch_state_t Patch::get_state()
{
    return this->state;
}

void Patch::set_state(signal_patch_state_t state)
{
    this->state = state;
}

NodeRef Patch::instantiate(NodeSpec *nodespec)
{
    /*------------------------------------------------------------------------
     * Recursively instantiate the subgraph specified in NodeSpec.
     * Does not currently support graphs that route one node to multiple
     * inputs.
     *-----------------------------------------------------------------------*/
    NodeRegistry *registry = NodeRegistry::global();

    NodeRef noderef;

    if (!nodespec->input_name.empty() && this->inputs[nodespec->input_name])
    {
        noderef = this->inputs[nodespec->input_name];
    }
    else
    {
        Node *node = registry->create(nodespec->name);
        noderef = NodeRef(node);

        /*------------------------------------------------------------------------
         * Update the patch's internal collection of node refs.
         *-----------------------------------------------------------------------*/
        this->nodes.insert(noderef);

        for (auto param : nodespec->params)
        {
            std::string param_name = param.first;
            NodeRef param_node = this->instantiate(param.second);
            noderef->set_input(param_name, param_node);
        }

        if (nodespec->is_constant)
        {
            Constant *constant = (Constant *) node;
            constant->value = nodespec->value;
        }

        if (!nodespec->input_name.empty())
        {
            this->inputs[nodespec->input_name] = noderef;
        }

        noderef->set_patch(this);
    }

    return noderef;
}

void Patch::set_input(std::string name, NodeRef value)
{
    /*------------------------------------------------------------------------
     * Replace a named input with another node.
     * Iterate over this patch's nodes, replacing the prior input with
     * the new node. (Inefficient, should be rethought.)
     *-----------------------------------------------------------------------*/
    if (this->inputs[name] == nullptr)
    {
        throw std::runtime_error("Patch has no such parameter: " + name);
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

void Patch::disconnect()
{
    this->graph->remove_output(this);
}

bool Patch::get_auto_free()
{
    return this->auto_free;
}
void Patch::set_auto_free(bool value)
{
    this->auto_free = value;
}

void Patch::node_state_changed(Node *node)
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

NodeRef Patch::add_input(std::string name, sample default_value)
{
    NodeRef placeholder(default_value);
    this->inputs[name] = placeholder;
    nodes.insert(placeholder);
    return placeholder;
}

std::string Patch::_get_input_name(const NodeRef &node)
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

NodeRef Patch::add_node(NodeRef node)
{
    nodes.insert(node);
    return node;
}

void Patch::set_output(NodeRef out)
{
    // check if out is in patch
    this->output = out;
}

/*------------------------------------------------------------------------
 * Scans the patch graph beginning from its outputs.
 *-----------------------------------------------------------------------*/
PatchSpecRef Patch::parse()
{
    // TODO: Currently have parsed property in this object and spec
    if (this->output == nullptr)
    {
        throw std::runtime_error("Patch " + this->name + ": output is not set");
    }
    NodeRef root = this->output;
    this->last_id = 0;

    PatchSpecRef spec = new PatchSpec(this->name);
    spec->output_def = this->_parse_from_node(root);
    spec->parsed = true;
    spec->nodespecs = this->nodespecs;
    for (auto node : nodes)
    {
        if (parsed_nodes.find(node) == parsed_nodes.end())
        {
            throw std::runtime_error("Patch contains unconnected node (" + node->name + ").");
        }
    }

    return spec;
}

NodeSpec Patch::_parse_from_node(const NodeRef &node)
{
    NodeSpec def(node->name);
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
                NodeSpec param_def = this->_parse_from_node(param_node);
                def.add_input(param.first, &param_def);
            }
        }
    }

    std::string input_name = this->_get_input_name(node);
    if (!input_name.empty())
    {
        def.input_name = input_name;
    }

    this->nodespecs[def.id] = def;
    this->parsed_nodes.insert(node);

    return def;
}

}
