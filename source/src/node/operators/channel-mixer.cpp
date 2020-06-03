#include "signal/core/core.h"
#include "signal/node/operators/channel-mixer.h"

namespace libsignal
{

ChannelMixer::ChannelMixer(int channels, NodeRef input)
    : UnaryOpNode(input), channels(channels)
{
    this->name = "channel-mixer";
    this->update_channels();
}

void ChannelMixer::process(sample **out, int num_frames)
{
    float out_channel_pan,
        in_channel_pan;

    this->zero_output();

    for (int out_channel = 0; out_channel < this->channels; out_channel++)
    {
        /*------------------------------------------------------------------------
         * out_channel_pan ranges from [0..1], where
         * leftmost channel = 0, rightmost channel = 1
         *-----------------------------------------------------------------------*/
        if (this->channels > 1)
            out_channel_pan = map(out_channel, 0, this->channels - 1, 0, 1);
        else
            out_channel_pan = 0.5;

        for (int in_channel = 0; in_channel < this->num_input_channels; in_channel++)
        {
            float channel_amp = 1.0;
            if (this->channels > 1)
            {
                if (this->num_input_channels > 1)
                    in_channel_pan = map(in_channel, 0, this->num_input_channels - 1, 0, 1);
                else
                    in_channel_pan = 0.5;

                float channel_distance = fabs(in_channel_pan - out_channel_pan);
                float channel_distance_max = 1.0 / (this->channels - 1);
                channel_amp = map(channel_distance,
                                  channel_distance_max, 0,
                                  0, 1);
                channel_amp = clip(channel_amp, 0, 1);
            }
            channel_amp = channel_amp * this->amp_compensation;

            for (int frame = 0; frame < num_frames; frame++)
            {
                out[out_channel][frame] += channel_amp * this->input->out[in_channel][frame];
            }
        }
    }
}

void ChannelMixer::update_channels()
{
    this->num_input_channels = this->min_input_channels = this->max_input_channels = this->input->num_output_channels;
    this->num_output_channels = this->min_output_channels = this->max_output_channels = this->channels;
    this->amp_compensation = (float) this->num_output_channels / this->num_input_channels;
    if (this->amp_compensation > 1.0)
    {
        this->amp_compensation = 1.0;
    }

    signal_debug("Node mixer set num_out_channels to %d, num_in_channels %d\n",
                 this->num_output_channels, this->num_input_channels);
}

}
