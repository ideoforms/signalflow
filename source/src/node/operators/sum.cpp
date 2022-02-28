#include "signalflow/core/core.h"
#include "signalflow/node/operators/sum.h"
#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

Sum::Sum()
{
    this->name = "sum";
    this->has_variable_inputs = true;
}

Sum::Sum(std::initializer_list<NodeRef> inputs)
    : Sum()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

Sum::Sum(std::vector<NodeRef> inputs)
    : Sum()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

Sum::Sum(std::vector<float> inputs)
    : Sum()
{
    for (float input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

Sum::Sum(std::vector<int> inputs)
    : Sum()
{
    for (int input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

void Sum::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        memset(this->out[channel], 0, sizeof(sample) * num_frames);
        for (NodeRef input : this->input_list)
        {
            for (int frame = 0; frame < num_frames; frame++)
            {
                out[channel][frame] += input->out[channel][frame];
            }
        }
    }
}

void Sum::add_input(NodeRef input)
{
    this->input_list.push_back(input);
    std::string input_name = "input" + std::to_string(this->input_index++);
    this->Node::create_input(input_name, input_list.back());
}

void Sum::remove_input(NodeRef node)
{
    // Copied from audioout_abstract
    // TODO: Apply this to a general VariableInputNode class
    bool removed = false;
    for (auto param : this->inputs)
    {
        if (*(param.second) == node)
        {
            /*--------------------------------------------------------------------------------
             * Remove Node outgoing reference first, to avoid node being garbage collected.
             *--------------------------------------------------------------------------------*/
            node->remove_output(this, param.first);

            this->destroy_input(param.first);
            this->input_list.remove(node);
            removed = true;
            break;
        }
    }
    if (!removed)
    {
        std::cerr << "Couldn't find node to remove" << std::endl;
    }
}

void Sum::set_input(std::string name, const NodeRef &node)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        this->input_list.push_back(node);
        this->Node::create_input(name, input_list.back());
    }

    this->Node::set_input(name, node);
}

}
