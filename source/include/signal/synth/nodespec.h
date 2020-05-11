#pragma once

#include <string>
#include <unordered_map>

namespace libsignal
{

class NodeSpec
{
public:
    NodeSpec();
    ~NodeSpec();
    NodeSpec(std::string name);
    NodeSpec(std::string name, float value);

    void set_id(int value);
    void add_input(std::string name, NodeSpec *def);
    void add_input(std::string name, float value);
    void set_name(std::string name);
    void set_value(float value);

    std::string name;
    float value;
    bool is_constant = false;
    std::unordered_map<std::string, NodeSpec *> params;

    int id;
    std::string input_name;
};

}
