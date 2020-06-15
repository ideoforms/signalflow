#include "signal/node/filters/pan.h"

namespace libsignal
{

LinearPanner::LinearPanner(int channels, NodeRef input, NodeRef pan)
    : input(input), pan(pan)
{
    this->num_input_channels = 1;
    this->num_output_channels = channels;

    this->min_input_channels = this->max_input_channels = 1;
    this->min_output_channels = this->max_output_channels = channels;

    this->add_input("input", this->input);
    this->add_input("pan", this->pan);

    this->name = "linear-panner";
}

void LinearPanner::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pan = this->pan->out[0][frame];
        sample in = this->input->out[0][frame];

        if (pan < 0)
            pan = 0.0f;
        if (pan > 1)
            pan = 1.0;

        if (this->num_output_channels == 2)
        {
            out[0][frame] = in * (1.0 - pan);
            out[1][frame] = in * pan;
        }
        else if (this->num_output_channels == 4)
        {
            // TODO calculate channels frames based on pan as angle in radians
        }
    }
}

}
