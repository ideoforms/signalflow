#include "signal/node/operators/multiplex.h"
#include "signal/node/oscillators/constant.h"
#include "signal/core/core.h"

namespace libsignal
{

Multiplex::Multiplex()
{
    this->name = "multiplex";
    this->no_input_upmix = true;
}

Multiplex::Multiplex(std::initializer_list<NodeRef> inputs)
    : Multiplex()
{
    if (inputs.size() > SIGNAL_MAX_CHANNELS)
    {
        throw std::runtime_error("Multiplex: Input has too many channels");
    }

    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

Multiplex::Multiplex(std::vector<NodeRef> inputs)
    : Multiplex()
{
    if (inputs.size() > SIGNAL_MAX_CHANNELS)
    {
        throw std::runtime_error("Multiplex: Input has too many channels");
    }

    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

Multiplex::Multiplex(std::vector<float> inputs)
    : Multiplex()
{
    if (inputs.size() > SIGNAL_MAX_CHANNELS)
    {
        throw std::runtime_error("Multiplex: Input has too many channels");
    }

    for (float input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

Multiplex::Multiplex(std::vector<int> inputs)
    : Multiplex()
{
    if (inputs.size() > SIGNAL_MAX_CHANNELS)
    {
        throw std::runtime_error("Multiplex: Input has too many channels");
    }

    for (int input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

void Multiplex::process(sample **out, int num_frames)
{
    int global_channel = 0;
    for (NodeRef input : this->input_list)
    {
        for (int this_channel = 0; this_channel < input->num_output_channels; this_channel++)
        {
            memcpy(out[global_channel + this_channel], input->out[this_channel], num_frames * sizeof(sample));
        }
        global_channel += input->num_output_channels;
    }
}

void Multiplex::update_channels()
{
    this->num_input_channels = 0;
    for (NodeRef input : this->input_list)
    {
        this->num_input_channels += input->num_output_channels;
    }

    this->num_output_channels = this->num_input_channels;

    this->min_input_channels = this->max_input_channels = this->num_input_channels;
    this->min_output_channels = this->max_output_channels = this->num_output_channels;

    signal_debug("Node %s set num_out_channels to %d", this->name.c_str(), this->num_output_channels);
}

void Multiplex::add_input(NodeRef input)
{
    this->input_list.push_back(input);
    std::string input_name = "input" + std::to_string(this->inputs.size());
    ;
    this->Node::add_input(input_name, input_list.back());
}

void Multiplex::set_input(std::string name, const NodeRef &node)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        this->input_list.push_back(node);
        this->Node::add_input(name, input_list.back());
    }

    this->Node::set_input(name, node);
}

}
