#include "signalflow/patch/patchnodespec.h"

namespace signalflow
{
//PatchNodeSpec::PatchNodeSpec()
//    : name("(null)")
//{
//}

PatchNodeSpec::~PatchNodeSpec()
{
    // free inputs
}

PatchNodeSpec::PatchNodeSpec(std::string name)
    : name(name)
{
}

PatchNodeSpec::PatchNodeSpec(std::string name, float value)
    : name(name)
{
    this->set_constant_value(value);
}

void PatchNodeSpec::set_id(int value)
{
    this->id = value;
}

void PatchNodeSpec::set_name(std::string name)
{
    this->name = name;
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

void PatchNodeSpec::set_constant_value(float value)
{
    this->value = value;
    this->is_constant = true;
}

}
