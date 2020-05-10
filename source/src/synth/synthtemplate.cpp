#include "signal/synth/synthtemplate.h"

#include "signal/core/core.h"
#include "signal/node/oscillators/constant.h"

namespace libsignal
{
SynthTemplate::SynthTemplate(std::string name)
{
    this->name = name;
}

/*------------------------------------------------------------------------
 * TEMPLATING
 *-----------------------------------------------------------------------*/

NodeRef SynthTemplate::add_input(std::string name, sample default_value)
{
    NodeRef placeholder(default_value);
    this->inputs[name] = placeholder.get();
    nodes.insert(placeholder);
    return placeholder;
}

std::string SynthTemplate::get_input_name(const NodeRef &node)
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

NodeRef SynthTemplate::add_node(NodeRef node)
{
    nodes.insert(node);
    return node;
}

void SynthTemplate::set_output(const NodeRef &out)
{
    this->output = out;
}

/*------------------------------------------------------------------------
 * Scans the graph synthtemplate beginning from its inputs and outputs.
 *-----------------------------------------------------------------------*/
SynthSpecRef SynthTemplate::parse()
{
    // TODO: Currently have parsed property in this object and spec
    if (!this->parsed)
    {
        if (this->output == nullptr)
        {
            throw std::runtime_error("SynthTemplate " + this->name + ": output is not set");
        }
        const NodeRef &root = this->output;
        this->last_id = 0;

        this->spec = new SynthSpec(this->name);
        spec->output_def = this->parse_root(root);
        spec->parsed = true;
        spec->nodedefs = this->nodedefs;
        this->parsed = true;
        for (auto node : nodes)
        {
            if (parsed_nodes.find(node) == parsed_nodes.end())
            {
                throw std::runtime_error("SynthTemplate contains unconnected node (" + node->name + ").");
            }
        }
    }
    return this->spec;
}

NodeDefinition SynthTemplate::parse_root(const NodeRef &node)
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
                NodeDefinition param_def = this->parse_root(param_node);
                def.add_input(param.first, &param_def);
            }
        }
    }

    std::string input_name = this->get_input_name(node);
    if (!input_name.empty())
    {
        def.input_name = input_name;
    }

    this->nodedefs[def.id] = def;
    this->parsed_nodes.insert(node);

    return def;
}
}
