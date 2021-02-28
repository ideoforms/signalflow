#include "signalflow/patch/patch.h"

#include "signalflow/core/core.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/patch/patch-registry.h"

#include "signalflow/node/operators/add.h"
#include "signalflow/node/operators/divide.h"
#include "signalflow/node/operators/multiply.h"
#include "signalflow/node/operators/subtract.h"

#include <iostream>
#include <memory>

namespace signalflow
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
    PatchNodeSpec *nodespec = patchspec->get_root();
    this->output = this->instantiate(nodespec);
    this->parsed = true;
}

Patch::Patch(PatchSpecRef patchspec, std::unordered_map<std::string, NodeRef> inputs)
    : Patch(patchspec)
{
    for (auto input : inputs)
    {
        std::string input_name = input.first;
        NodeRef input_value = input.second;
        this->set_input(input_name, input_value);
    }
}

Patch::Patch(PatchRef patch)
    : Patch(patch->create_spec())
{
}

Patch::Patch(PatchRef patch, std::unordered_map<std::string, NodeRef> inputs)
    : Patch(patch->create_spec(), inputs)
{
}

Patch::Patch(std::string name)
    : Patch()
{
    PatchSpecRef patchspec = PatchRegistry::global()->get(name);
    if (patchspec)
    {
        PatchNodeSpec *nodespec = patchspec->get_root();
        this->output = this->instantiate(nodespec);
    }
}

Patch::~Patch()
{
}

signalflow_patch_state_t Patch::get_state()
{
    return this->state;
}

void Patch::set_state(signalflow_patch_state_t state)
{
    this->state = state;
}

NodeRef Patch::instantiate(PatchNodeSpec *nodespec)
{
    /*------------------------------------------------------------------------
     * Recursively instantiate the subgraph specified in PatchNodeSpec.
     * Does not currently support graphs that route one node to multiple
     * inputs.
     *-----------------------------------------------------------------------*/
    NodeRegistry *registry = NodeRegistry::global();
    NodeRef noderef;

    if (!nodespec->get_input_name().empty() && this->inputs[nodespec->get_input_name()])
    {
        noderef = this->inputs[nodespec->get_input_name()];
    }
    else
    {
        Node *node = registry->create(nodespec->get_name());
        noderef = NodeRef(node);

        /*------------------------------------------------------------------------
         * Update the patch's internal collection of node refs.
         *-----------------------------------------------------------------------*/
        this->nodes.insert(noderef);

        for (auto input : nodespec->get_inputs())
        {
            std::string input_name = input.first;
            NodeRef input_node = this->instantiate(input.second);
            noderef->set_input(input_name, input_node);
        }

        if (nodespec->get_is_constant())
        {
            Constant *constant = (Constant *) node;
            constant->value = nodespec->get_constant_value();
        }

        if (!nodespec->get_input_name().empty())
        {
            this->inputs[nodespec->get_input_name()] = noderef;
        }

        for (auto buffer_input : nodespec->get_buffer_inputs())
        {
            std::string patch_input_name = buffer_input.first;
            std::string node_input_name = buffer_input.second;
            BufferRef placeholder = new Buffer();
            noderef->set_buffer(node_input_name, placeholder);
            this->buffer_inputs[patch_input_name] = placeholder;
        }

        noderef->set_patch(this);
    }

    return noderef;
}

void Patch::set_input(std::string name, float value)
{
    if (this->inputs[name] == nullptr)
    {
        throw std::runtime_error("Patch has no such parameter: " + name);
    }
    NodeRef current = this->inputs[name];
    if (current->name == "constant")
    {
        Constant *current_constant = (Constant *) current.get();
        current_constant->value = value;
    }
    else
    {
        printf("name: %s\n", current->name.c_str());
    }
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
    bool found_input = false;
    for (NodeRef node : this->nodes)
    {
        for (auto param : node->inputs)
        {
            if ((param.second)->get() == current.get())
            {
                // Update routing
                node->set_input(param.first, value);
                found_input = true;
            }
        }
    }
    if (!found_input)
    {
        throw std::runtime_error("Couldn't find input: " + name);
    }
    this->inputs[name] = value;
}

void Patch::set_input(std::string name, BufferRef value)
{
    if (this->buffer_inputs[name] == nullptr)
    {
        throw std::runtime_error("Patch has no such buffer parameter: " + name);
    }
    BufferRef current = this->buffer_inputs[name];
    for (NodeRef node : this->nodes)
    {
        for (auto param : node->buffers)
        {
            if ((param.second)->get() == current.get())
            {
                // Update routing
                node->set_buffer(param.first, value);
            }
        }
    }
}

void Patch::disconnect()
{
    this->graph->stop(this);
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
    if (node->get_state() == SIGNALFLOW_NODE_STATE_STOPPED && this->auto_free)
    {
        this->set_state(SIGNALFLOW_PATCH_STATE_STOPPED);
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

BufferRef Patch::add_buffer_input(std::string name)
{
    BufferRef placeholder = new Buffer();
    this->buffer_inputs[name] = placeholder;
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

std::string Patch::_get_input_name(const BufferRef &buffer)
{
    for (auto input : this->buffer_inputs)
    {
        std::string name = input.first;
        BufferRef buffer_ptr = input.second;
        if (buffer_ptr.get() == buffer.get())
            return name;
    }

    return "";
}

NodeRef Patch::add_node(NodeRef node)
{
    nodes.insert(node);
    node->patch = this;
    return node;
}
void Patch::set_output(NodeRef out)
{
    /*----------------------------------------------------------------------------
     * Scans the patch graph beginning from its outputs.
     *----------------------------------------------------------------------------*/
    this->output = out;
    this->add_node(out);
}

AudioGraph *Patch::get_graph()
{
    return this->graph;
}

void Patch::parse()
{
    if (!this->parsed)
    {
        if (this->output == nullptr)
        {
            throw std::runtime_error("Patch does not have an output set");
        }
        this->_iterate_from_node(this->output);
        this->parsed = true;
        signalflow_debug("Parsed patch (total %lu nodes)\n", this->nodes.size());
    }
}

/*----------------------------------------------------------------------------
 * Scans the patch graph beginning from its outputs.
 *----------------------------------------------------------------------------*/
PatchSpecRef Patch::create_spec()
{
    // TODO: Currently have parsed property in this object and spec
    if (this->output == nullptr)
    {
        throw std::runtime_error("Patch " + this->name + ": output is not set");
    }
    NodeRef root = this->output;
    this->last_id = 0;

    PatchSpecRef spec = new PatchSpec();
    spec->set_name(this->name);
    spec->output = this->_create_spec_from_node(root);
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

void Patch::_iterate_from_node(const NodeRef &node)
{
    for (auto input : node->inputs)
    {
        NodeRef input_node = *(input.second);
        if (input_node)
        {
            if (nodes.find(input_node) == nodes.end())
            {
                // Lazy - use a better way of checking for constant
                if (input_node->name != "constant")
                {
                    this->add_node(input_node);
                    this->_iterate_from_node(input_node);
                }
            }
        }
    }
}

PatchNodeSpec *Patch::_create_spec_from_node(const NodeRef &node)
{
    PatchNodeSpec *nodespec = new PatchNodeSpec(node->name);
    nodespec->set_id(this->last_id++);

    if (node->name == "constant")
    {
        Constant *constant = (Constant *) node.get();
        nodespec->set_constant_value(constant->value);
    }
    else
    {
        for (auto input : node->inputs)
        {
            NodeRef input_node = *(input.second);
            if (input_node)
            {
                PatchNodeSpec *input_spec = this->_create_spec_from_node(input_node);
                nodespec->add_input(input.first, input_spec);
            }
        }

        for (auto buffer : node->buffers)
        {
            BufferRef input_buffer = *(buffer.second);
            std::string buffer_input_name = this->_get_input_name(input_buffer);
            if (!buffer_input_name.empty())
            {
                nodespec->add_buffer_input(buffer_input_name, buffer.first);
            }
        }
    }

    std::string input_name = this->_get_input_name(node);
    if (!input_name.empty())
    {
        nodespec->set_input_name(input_name);
    }

    this->nodespecs[nodespec->get_id()] = nodespec;
    this->parsed_nodes.insert(node);

    return this->nodespecs[nodespec->get_id()];
}

template <class T>
NodeRef PatchRefTemplate<T>::operator*(NodeRef other)
{
    return new Multiply(this->get()->output, other);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator*(double constant)
{
    return new Multiply(this->get()->output, constant);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator+(NodeRef other)
{
    return new Add(this->get()->output, other);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator+(double constant)
{
    return new Add(this->get()->output, constant);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator-(NodeRef other)
{
    return new Subtract(this->get()->output, other);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator-(double constant)
{
    return new Subtract(this->get()->output, constant);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator/(NodeRef other)
{
    return new Divide(this->get()->output, other);
}

template <class T>
NodeRef PatchRefTemplate<T>::operator/(double constant)
{
    return new Divide(this->get()->output, constant);
}

template class PatchRefTemplate<Patch>;

}
