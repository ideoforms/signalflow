#include "signalflow/node/operators/channel-crossfade.h"

namespace signalflow
{

ChannelCrossfade::ChannelCrossfade(NodeRef input, NodeRef index, int num_output_channels)
    : UnaryOpNode(input), index(index), num_output_channels(num_output_channels)
{
    if (!input)
    {
        throw std::runtime_error("ChannelCrossfade: No input specified");
    }

    this->name = "channel-crossfade";

    this->create_input("index", this->index);

    this->set_channels(this->input->get_num_output_channels(), num_output_channels);
}

void ChannelCrossfade::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float input_channel_index = this->index->out[0][frame];
        int input_channel_index_int = (int) input_channel_index;
        float xfade = input_channel_index - input_channel_index_int;

        for (int output_channel = 0; output_channel < this->num_output_channels; output_channel++)
        {
            if (input_channel_index + output_channel < this->num_input_channels - 1)
            {
                float in_a = this->input->out[input_channel_index + output_channel][frame];
                float in_b = this->input->out[input_channel_index + output_channel + 1][frame];
                out[output_channel][frame] = signalflow_interpolate_equal_power(in_a, in_b, xfade);
            }
            else
            {
                out[output_channel][frame] = 0;
            }
        }
    }
}

}
