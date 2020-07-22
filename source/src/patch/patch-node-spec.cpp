#include "signalflow/patch/patch-node-spec.h"

namespace signalflow
{
PatchNodeSpec::PatchNodeSpec(std::string name)
    : name(name)
{
}

PatchNodeSpec::PatchNodeSpec(std::string name, float value)
    : name(name)
{
    this->set_constant_value(value);
}

PatchNodeSpec::~PatchNodeSpec()
{
    // free inputs
}

int PatchNodeSpec::get_id()
{
    return this->id;
}

void PatchNodeSpec::set_id(int value)
{
    this->id = value;
}

std::string PatchNodeSpec::get_name()
{
    return this->name;
}

void PatchNodeSpec::set_name(std::string name)
{
    this->name = name;
}

bool PatchNodeSpec::get_is_constant()
{
    return this->is_constant;
}

float PatchNodeSpec::get_constant_value()
{
    return this->value;
}

void PatchNodeSpec::set_constant_value(float value)
{
    this->value = value;
    this->is_constant = true;
}

void PatchNodeSpec::add_input(std::string name, PatchNodeSpec *def)
{
    PatchNodeSpec *def_copy = new PatchNodeSpec();
    *def_copy = *def;
    this->inputs[name] = def_copy;
}

void PatchNodeSpec::add_input(std::string name, float value)
{
    this->inputs[name] = new PatchNodeSpec("constant", value);
}

void PatchNodeSpec::add_buffer_input(std::string patch_input_name, std::string node_input_name)
{
    this->buffer_inputs[patch_input_name] = node_input_name;
}

std::unordered_map<std::string, PatchNodeSpec *> PatchNodeSpec::get_inputs()
{
    return this->inputs;
}

std::unordered_map<std::string, std::string> PatchNodeSpec::get_buffer_inputs()
{
    return this->buffer_inputs;
}

void PatchNodeSpec::set_input_name(std::string name)
{
    this->input_name = name;
}

std::string PatchNodeSpec::get_input_name()
{
    return this->input_name;
}

}
