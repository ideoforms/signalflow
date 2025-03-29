#include "signalflow/core/core.h"
#include "signalflow/node/operators/channel-array.h"

namespace signalflow
{

void ChannelArray::init()
{
    this->name = "channel-array";
    this->no_input_upmix = true;

    /*--------------------------------------------------------------------------------*
     * This explicit call to update_channels() is needed because C++ cannot call
     * this class's update_channels() method in the superclass constructor.
     *
     * When inputs are created in VariableInputNode's constructor, this should normally
     * happen automatically. Instead we need to do it here explicitly.
     *---------------------------------------------------------------------------------*/
    this->update_channels();
}

ChannelArray::ChannelArray()
    : VariableInputNode()
{
    this->init();
}

ChannelArray::ChannelArray(std::initializer_list<NodeRef> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

ChannelArray::ChannelArray(std::vector<NodeRef> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

ChannelArray::ChannelArray(std::vector<float> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

ChannelArray::ChannelArray(std::vector<int> inputs)
    : VariableInputNode(inputs)
{
    this->init();
}

void ChannelArray::process(Buffer &out, int num_frames)
{
    int global_channel = 0;
    for (NodeRef input : this->input_list)
    {
        for (int this_channel = 0; this_channel < input->get_num_output_channels(); this_channel++)
        {
            memcpy(out[global_channel + this_channel], input->out[this_channel], num_frames * sizeof(sample));
        }
        global_channel += input->get_num_output_channels();
    }
}

void ChannelArray::update_channels()
{
    this->num_input_channels = 0;
    for (NodeRef input : this->input_list)
    {
        this->num_input_channels += input->get_num_output_channels();
    }

    this->set_channels(this->num_input_channels, this->num_input_channels);
}

}
