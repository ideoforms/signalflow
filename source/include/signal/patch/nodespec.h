#pragma once

#include <string>
#include <unordered_map>

namespace libsignal
{

class BufferSpec;

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
    void add_buffer_input(std::string name);
    void set_name(std::string name);
    void set_value(float value);

    std::string name;
    float value;
    bool is_constant = false;
    std::unordered_map<std::string, NodeSpec *> inputs;
    std::unordered_map<std::string, BufferSpec *> buffer_inputs;

    int id;
    std::string input_name;
};

class BufferSpec
{
public:
    BufferSpec() {}
    BufferSpec(std::string name)
        : name(name) {};

    std::string name;
};

}
