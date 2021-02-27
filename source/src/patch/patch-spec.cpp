#include "signalflow/patch/patch-spec.h"

#include "json11/json11.hpp"
#include "signalflow/core/core.h"
#include "signalflow/node/registry.h"
#include "signalflow/patch/patch-registry.h"

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace json11;

namespace signalflow
{

PatchSpec::PatchSpec()
{
    this->name = "Unnamed Patch";
}

PatchSpec::PatchSpec(std::string filename)
    : PatchSpec()
{
    this->load(filename);
}

void PatchSpec::load(std::string filename)
{
    std::string buf;
    std::string line;

    std::ifstream input(filename);
    if (!input.good())
    {
        /*--------------------------------------------------------------------------------
         * If absolute pathname couldn't be found, search in user's patches directory
         * (~/.signalflow/patches)
         *--------------------------------------------------------------------------------*/
        input.open(SIGNALFLOW_USER_DIR + "/patches/" + filename);
        if (!input.good())
        {
            throw std::runtime_error("Couldn't read from patch file: " + filename);
        }
    }
    while (std::getline(input, line))
    {
        buf += line + "\n";
    }
    this->from_json(buf);
}

void PatchSpec::save(std::string filename)
{
    std::ofstream output(filename);
    std::string buf = this->to_json();
    output << buf;
    output.close();
}

void PatchSpec::from_json(std::string buf)
{
    std::string err;

    auto json = Json::parse(buf, err);
    if (!err.empty())
    {
        throw std::runtime_error("An error occurred when parsing JSON: " + err);
    }

    if (!json.is_object())
    {
        throw std::runtime_error("Cannot parse JSON (root element must be object)");
    }

    this->name = json["name"].string_value();

    auto nodes_list = json["nodes"];
    for (auto node_obj : nodes_list.array_items())
    {
        PatchNodeSpec *nodespec = new PatchNodeSpec();
        bool is_output = false;

        if (node_obj["id"].is_null() || node_obj["node"].is_null())
        {
            throw std::runtime_error("Cannot parse JSON (node has a null name/id)");
        }

        nodespec->set_name(node_obj["node"].string_value());
        nodespec->set_id(node_obj["id"].int_value());
        if (node_obj["is_output"].bool_value())
        {
            is_output = true;
        }
        auto inputs = node_obj["inputs"];

        for (auto input_pair : inputs.object_items())
        {
            std::string input_key = input_pair.first;
            auto input_value = input_pair.second;

            if (input_value.is_number())
            {
                nodespec->add_input(input_key, input_value.number_value());
            }
            else if (input_value.is_object())
            {
                int id = input_value["id"].int_value();
                PatchNodeSpec *ptr = this->get_node_spec(id);
                nodespec->add_input(input_key, ptr);
            }
        }

        signalflow_debug("Adding node with name %s\n", nodespec->get_name().c_str());
        this->add_node_spec(nodespec);
        if (is_output)
        {
            this->set_output(nodespec);
        }
    }

    if (this->output->get_id() == -1)
    {
        throw std::runtime_error("Invalid JSON (no output specified)");
    }

    auto inputs = json["inputs"];
    for (auto input : inputs.array_items())
    {
        if (input["node_id"].is_null() || input["patch_input_name"].is_null() || input["node_input_name"].is_null())
        {
            throw std::runtime_error("Input specification is incomplete");
        }
        auto node_id = input["node_id"].int_value();
        auto patch_input_name = input["patch_input_name"].string_value();
        auto node_input_name = input["node_input_name"].string_value();

        if (this->nodespecs.find(node_id) == this->nodespecs.end())
        {
            throw std::runtime_error("Patch input '" + patch_input_name + "': Can't find node ID " + std::to_string(node_id));
        }
        auto node_inputs = this->nodespecs[node_id]->get_inputs();
        if (node_inputs.find(node_input_name) == node_inputs.end())
        {
            throw std::runtime_error("Patch input '" + patch_input_name + "': node ID " + std::to_string(node_id) + " does not have an input named '" + node_input_name + "'");
        }
        auto input_node = node_inputs[node_input_name];
        input_node->set_input_name(patch_input_name);
    }

    auto buffer_inputs = json["buffer_inputs"];
    for (auto input : buffer_inputs.array_items())
    {
        if (input["node_id"].is_null() || input["patch_input_name"].is_null() || input["node_input_name"].is_null())
        {
            throw std::runtime_error("Buffer input specification is incomplete");
        }

        auto patch_input_name = input["patch_input_name"].string_value();
        auto node_input_name = input["node_input_name"].string_value();
        auto node_id = input["node_id"].int_value();
        auto input_node = this->nodespecs[node_id];
        input_node->add_buffer_input(patch_input_name, node_input_name);
    }
}

std::string PatchSpec::to_json()
{
    std::vector<Json::object> nodes;
    std::vector<Json::object> patch_inputs;
    std::vector<Json::object> patch_buffer_inputs;

    for (auto pair : nodespecs)
    {
        PatchNodeSpec *spec = pair.second;

        if (spec->get_name() == "constant")
        {
            continue;
        }

        Json::object object = {
            { "node", spec->get_name() },
            { "id", spec->get_id() },
        };
        if (spec->get_id() == this->output->get_id())
        {
            object["is_output"] = true;
        }
        Json::object inputs;
        for (auto input_pair : spec->get_inputs())
        {
            std::string input_name = input_pair.first;
            PatchNodeSpec *input_spec = input_pair.second;
            if (input_spec->get_is_constant())
            {
                inputs[input_name] = input_spec->get_constant_value();
            }
            else
            {
                inputs[input_name] = Json::object({ { "id", input_spec->get_id() } });
            }

            if (!input_spec->get_input_name().empty())
            {
                patch_inputs.push_back(Json::object({ { "patch_input_name", input_spec->get_input_name() },
                                                      { "node_input_name", input_name },
                                                      { "node_id", spec->get_id() } }));
            }
        }
        if (spec->get_buffer_inputs().size() > 0)
        {
            for (auto input_pair : spec->get_buffer_inputs())
            {
                std::string patch_input_name = input_pair.first;
                std::string node_input_name = input_pair.second;
                patch_buffer_inputs.push_back(Json::object({ { "patch_input_name", patch_input_name },
                                                             { "node_input_name", node_input_name },
                                                             { "node_id", spec->get_id() } }));
            }
        }
        if (inputs.size() > 0)
        {
            object["inputs"] = inputs;
        }
        nodes.push_back(object);
    }

    std::reverse(nodes.begin(), nodes.end());

    Json::object root;
    root["name"] = this->name;
    root["nodes"] = nodes;
    root["inputs"] = patch_inputs;
    root["buffer_inputs"] = patch_buffer_inputs;

    std::string json = Json(root).dump();
    return json;
}

void PatchSpec::store()
{
    PatchRegistry::global()->add(this->name, this);
}

void PatchSpec::add_node_spec(PatchNodeSpec *spec)
{
    this->nodespecs[spec->get_id()] = spec;
}

PatchNodeSpec *PatchSpec::get_node_spec(int id)
{
    return this->nodespecs[id];
}

void PatchSpec::set_output(PatchNodeSpec *spec)
{
    this->output = spec;
}

std::string PatchSpec::get_name()
{
    return this->name;
}

void PatchSpec::set_name(std::string name)
{
    this->name = name;
}

PatchNodeSpec *PatchSpec::get_root()
{
    return this->output;
}

void PatchSpec::print()
{
    std::cout << "PatchSpec " << this->name << " (" << this->nodespecs.size() << " nodes)" << std::endl;
    this->print(this->output, 0);
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
