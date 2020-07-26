#include "signalflow/node/io/output/abstract.h"

namespace signalflow
{

AudioOut_Abstract::AudioOut_Abstract()
{
    this->name = "audioout";
    this->set_channels(2, 0);
    this->no_input_upmix = true;
    this->input_index = 0;
}

void AudioOut_Abstract::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        memset(out[channel], 0, num_frames * sizeof(sample));
    }

    for (NodeRef input : this->audio_inputs)
    {
        for (int channel = 0; channel < input->num_output_channels; channel++)
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
            this->Node::destroy_input(param.first);
            removed = true;
            break;
        }
    }
    if (!removed)
    {
        throw std::runtime_error("Couldn't find node to remove");
    }
    audio_inputs.remove(node);
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
