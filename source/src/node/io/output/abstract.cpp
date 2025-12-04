#include "signalflow/node/io/output/abstract.h"
#include <algorithm>

namespace signalflow
{

AudioOut_Abstract::AudioOut_Abstract()
{
    this->name = "audioout";
    this->set_channels(2, 2);
    this->no_input_upmix = true;
    this->has_variable_inputs = true;
    this->input_index = 0;
    this->buffer_size = SIGNALFLOW_DEFAULT_BLOCK_SIZE;
}

void AudioOut_Abstract::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        memset(out[channel], 0, num_frames * sizeof(sample));
    }

    for (NodeRef input : this->audio_inputs)
    {
        for (int channel = 0; channel < input->get_num_output_channels(); channel++)
        {
#ifdef __APPLE__

            vDSP_vadd(input->out[channel], 1, out[channel], 1, out[channel], 1, num_frames);

#else

            for (int frame = 0; frame < num_frames; frame++)
            {
                out[channel][frame] += input->out[channel][frame];
            }

#endif
        }
    }
}

void AudioOut_Abstract::add_input(NodeRef node)
{
    if (this->has_input(node))
    {
        throw node_already_playing_exception();
    }

    audio_inputs.push_back(node);
    std::string input_name = "input" + std::to_string(input_index);
    this->input_index++;
    this->Node::create_input(input_name, audio_inputs.back());
}

void AudioOut_Abstract::remove_input(NodeRef node)
{
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
            audio_inputs.remove(node);
            removed = true;
            break;
        }
    }
    if (!removed)
    {
        std::cerr << "Couldn't find node to remove" << std::endl;
    }
}

void AudioOut_Abstract::replace_input(NodeRef node, NodeRef other)
{
    bool replaced = false;
    for (auto param : this->inputs)
    {
        if (*(param.second) == node)
        {
            // Need to call create_input to also update the channel I/O on `other`
            audio_inputs.remove(node);
            audio_inputs.push_back(other);
            this->create_input(param.first, audio_inputs.back());
            replaced = true;
            break;
        }
    }

    if (!replaced)
    {
        std::cerr << "Couldn't find node to replace" << std::endl;
        // throw std::runtime_error("Couldn't find node to replace");
    }
}

void AudioOut_Abstract::set_channels(int num_input_channels, int num_output_channels, bool disable_input_channel_matching)
{
    Node::set_channels(num_input_channels, num_output_channels, disable_input_channel_matching);

    /*--------------------------------------------------------------------------------
     * Typically, Node objects allocate an output buffer per output channel.
     * In this unique case, allocate an output buffer per input channel.
     *--------------------------------------------------------------------------------*/
    this->resize_output_buffers(num_input_channels);
}

bool AudioOut_Abstract::has_input(NodeRef node)
{
    return std::find(std::begin(audio_inputs), std::end(audio_inputs), node) != std::end(audio_inputs);
}

std::list<NodeRef> AudioOut_Abstract::get_inputs()
{
    return this->audio_inputs;
}

unsigned int AudioOut_Abstract::get_sample_rate()
{
    return this->sample_rate;
}

unsigned int AudioOut_Abstract::get_buffer_size()
{
    return this->buffer_size;
}

} // namespace signalflow
