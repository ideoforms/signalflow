#include "signalflow/patch/patch-spec.h"

#include "json11/json11.hpp"
#include "signalflow/core/core.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/registry.h"
#include "signalflow/patch/patch-registry.h"

#include <fstream>
#include <iostream>

using namespace json11;

namespace signalflow
{
PatchSpec::PatchSpec(std::string name)
{
    this->name = name;
}

void PatchSpec::load(std::string filename)
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
        PatchNodeSpec nodespec;
        bool is_output = false;
        for (auto pair : element.object_items())
        {
            std::string key = pair.first;
            auto value = pair.second;

            if (key == "node")
            {
                nodespec.set_name(value.string_value());
            }
            else if (key == "id")
            {
                nodespec.set_id(value.int_value());
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
                        nodespec.add_input(input_key, input_value.number_value());
                    }
                    else if (input_value.is_object())
                    {
                        int id = input_value["id"].int_value();
                        PatchNodeSpec *ptr = this->get_node_spec(id);
                        nodespec.add_input(input_key, ptr);
                    }
                }
            }
        }
        signal_debug("Adding node with name %s\n", nodespec.get_name().c_str());
        this->add_node_spec(nodespec);
        if (is_output)
        {
            this->set_output(nodespec);
        }
    }
}

void PatchSpec::save(std::string filename)
{
}

void PatchSpec::store()
{
    PatchRegistry::global()->add(this->name, this);
}

void PatchSpec::add_node_spec(PatchNodeSpec spec)
{
    this->nodespecs[spec.get_id()] = spec;
}

PatchNodeSpec *PatchSpec::get_node_spec(int id)
{
    return &(this->nodespecs[id]);
}

void PatchSpec::set_output(PatchNodeSpec spec)
{
    this->output = spec;
}

std::string PatchSpec::get_name()
{
    return this->name;
}

PatchNodeSpec PatchSpec::get_root()
{
    return this->output;
}

void PatchSpec::print()
{
    std::cout << "PatchSpec " << this->name << " (" << this->nodespecs.size() << " nodes)" << std::endl;
    this->print(&this->output, 0);
}

void PatchSpec::print(PatchNodeSpec *root, int depth)
{
    std::cout << std::string(depth * 2, ' ');
    std::cout << " * " << root->get_name() << " (id = " << root->get_id() << ") " << std::endl;
    for (auto pair : root->get_inputs())
    {
        std::cout << std::string((depth + 1) * 2 + 1, ' ');

        if (pair.second->get_name() == "constant")
        {
            std::cout << pair.first << ": " << pair.second->get_constant_value();

            if (pair.second->get_input_name() != "")
            {
                std::cout << " (patch input: " << pair.second->get_input_name() << ")" << std::endl;
            }

            std::cout << std::endl;
        }
        else
        {
            std::cout << pair.first << ":" << std::endl;
            this->print(pair.second, depth + 1);
        }
    }
}

}
