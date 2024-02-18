#include "signalflow/core/core.h"
#include "signalflow/node/operators/channel-mixer.h"

namespace signalflow
{

ChannelMixer::ChannelMixer(int num_channels, NodeRef input, bool amplitude_compensation)
    : UnaryOpNode(input), num_channels(num_channels), amplitude_compensation(amplitude_compensation)
{
    this->name = "channel-mixer";
    this->amplitude_compensation_level = 1.0;

    this->create_property("num_channels", this->num_channels);
    this->update_channels();
}

void ChannelMixer::set_property(std::string name, const PropertyRef &value)
{
    Node::set_property(name, value);

    if (name == "num_channels")
    {
        this->update_channels();
    }
}

void ChannelMixer::process(Buffer &out, int num_frames)
{
    float out_channel_pan,
        in_channel_pan;

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        memset(out[channel], 0, num_frames * sizeof(sample));
    }

    int channels = this->num_channels->int_value();
    for (int out_channel = 0; out_channel < channels; out_channel++)
    {
        /*------------------------------------------------------------------------
         * out_channel_pan ranges from [0..1], where
         * leftmost channel = 0, rightmost channel = 1
         *-----------------------------------------------------------------------*/
        if (channels > 1)
            out_channel_pan = signalflow_scale_lin_lin(out_channel, 0, channels - 1, 0, 1);
        else
            out_channel_pan = 0.5;

        for (int in_channel = 0; in_channel < this->num_input_channels; in_channel++)
        {
            float channel_amp = 1.0;
            if (channels > 1)
            {
                if (this->num_input_channels > 1)
                    in_channel_pan = signalflow_scale_lin_lin(in_channel, 0, this->num_input_channels - 1, 0, 1);
                else
                    in_channel_pan = 0.5;

                float channel_distance = fabs(in_channel_pan - out_channel_pan);
                float channel_distance_max = 1.0 / (channels - 1);
                channel_amp = signalflow_scale_lin_lin(channel_distance,
                                                       channel_distance_max, 0,
                                                       0, 1);
                channel_amp = signalflow_clip(channel_amp, 0, 1);
            }
            channel_amp = channel_amp * this->amplitude_compensation_level;

#ifdef __APPLE__
            vDSP_vsma(this->input->out[in_channel], 1,
                      &channel_amp,
                      out[out_channel], 1,
                      out[out_channel], 1,
                      num_frames);
#else
            for (int frame = 0; frame < num_frames; frame++)
            {
                out[out_channel][frame] += channel_amp * this->input->out[in_channel][frame];
            }
#endif
        }
    }
}

void ChannelMixer::update_channels()
{
    /*--------------------------------------------------------------------------------
     * This logic needs to override update_channels, so that it is called successfully
     * in case `this->input` changes its number of output channels.
     *--------------------------------------------------------------------------------*/
    this->set_channels(this->input->get_num_output_channels(),
                       this->num_channels->int_value());

    if (this->amplitude_compensation)
    {
        this->amplitude_compensation_level = (float) this->num_output_channels / this->num_input_channels;
        if (this->amplitude_compensation_level > 1.0)
        {
            this->amplitude_compensation_level = 1.0;
        }
    }

    signalflow_debug("Node mixer set num_out_channels to %d, num_in_channels %d\n",
                     this->num_output_channels, this->num_input_channels);
}

}
