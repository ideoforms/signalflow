#include "signal/synth/synthspec.h"

#include "json11/json11.hpp"
#include "signal/core/core.h"
#include "signal/node/oscillators/constant.h"
#include "signal/node/registry.h"
#include "signal/synth/synthregistry.h"

#include <fstream>
#include <iostream>

using namespace json11;

namespace libsignal
{
SynthSpec::SynthSpec(std::string name)
{
    this->name = name;
}

void SynthSpec::save(std::string filename)
{
}

void SynthSpec::load(std::string filename)
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
        throw std::runtime_error("An error occurred when parsing JSON: " + err);
    }

    if (!json.is_array())
    {
        throw std::runtime_error("Cannot parse JSON (root element must be array)");
    }

    for (auto element : json.array_items())
    {
        NodeDefinition nodedef;
        bool is_output = false;
        for (auto pair : element.object_items())
        {
            std::string key = pair.first;
            auto value = pair.second;

            if (key == "node")
            {
                nodedef.set_name(value.string_value());
            }
            else if (key == "id")
            {
                nodedef.set_id(value.int_value());
            }
            else if (key == "is_output")
            {
                is_output = true;
            }
            else if (key == "inputs")
            {
                for (auto input_pair : value.object_items())
                {
                    std::string input_key = input_pair.first;
                    auto input_value = input_pair.second;

                    if (input_value.is_number())
                    {
                        nodedef.add_input(input_key, input_value.number_value());
                    }
                    else if (input_value.is_object())
                    {
                        int id = input_value["id"].int_value();
                        NodeDefinition *ptr = this->get_node_def(id);
                        nodedef.add_input(input_key, ptr);
                    }
                }
            }
        }
        signal_debug("Adding node with name %s\n", nodedef.name.c_str());
        this->add_node_def(nodedef);
        if (is_output)
        {
            this->set_output(nodedef);
        }
    }

    /*------------------------------------------------------------------------
     * Set `parsed` to indicate we have a complete NodeDef tree
     * (normally used to indicate the completion of template-based
     * construction)
     *-----------------------------------------------------------------------*/
    this->parsed = true;
}

void SynthSpec::store()
{
    SynthRegistry::global()->add(this->name, this);
}

void SynthSpec::add_node_def(NodeDefinition def)
{
    this->nodedefs[def.id] = def;
}

NodeDefinition *SynthSpec::get_node_def(int id)
{
    return &(this->nodedefs[id]);
}

void SynthSpec::set_output(NodeDefinition def)
{
    this->output_def = def;
}

NodeDefinition SynthSpec::get_root()
{
    return this->output_def;
}

void SynthSpec::print()
{
    std::cout << "SynthSpec " << this->name << " (" << this->nodedefs.size() << " nodes)" << std::endl;
    this->print(&this->output_def, 0);
}

void SynthSpec::print(NodeDefinition *root, int depth)
{
    std::cout << std::string(depth * 2, ' ');
    std::cout << " * " << root->name << " (id = " << root->id << ") " << std::endl;
    for (auto pair : root->params)
    {
        std::cout << std::string((depth + 1) * 2 + 1, ' ');

        if (pair.second->name == "constant")
        {
            std::cout << pair.first << ": " << pair.second->value << std::endl;
        }
        else
        {
            std::cout << pair.first << ":" << std::endl;
            this->print(pair.second, depth + 1);
        }
    }
}

}
