#include "signalflow/core/core.h"
#include "signalflow/node/operators/channel-array.h"
#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

ChannelArray::ChannelArray()
{
    this->name = "channel-array";
    this->no_input_upmix = true;
}

ChannelArray::ChannelArray(std::initializer_list<NodeRef> inputs)
    : ChannelArray()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

ChannelArray::ChannelArray(std::vector<NodeRef> inputs)
    : ChannelArray()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

ChannelArray::ChannelArray(std::vector<float> inputs)
    : ChannelArray()
{
    for (float input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

ChannelArray::ChannelArray(std::vector<int> inputs)
    : ChannelArray()
{
    for (int input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

void ChannelArray::process(sample **out, int num_frames)
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

void ChannelArray::update_channels()
{
    this->num_input_channels = 0;
    for (NodeRef input : this->input_list)
    {
        this->num_input_channels += input->num_output_channels;
    }

    this->set_channels(this->num_input_channels, this->num_input_channels);

    signal_debug("Node %s set num_out_channels to %d", this->name.c_str(), this->num_output_channels);

    this->allocate_output_buffer();
}

void ChannelArray::add_input(NodeRef input)
{
    this->input_list.push_back(input);
    std::string input_name = "input" + std::to_string(this->inputs.size());
    this->Node::create_input(input_name, input_list.back());
}

void ChannelArray::set_input(std::string name, const NodeRef &node)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        this->input_list.push_back(node);
        this->Node::create_input(name, input_list.back());
    }

    this->Node::set_input(name, node);
}

}
