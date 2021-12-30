#include "signalflow/node/processors/panning/pan.h"

namespace signalflow
{

LinearPanner::LinearPanner(int channels, NodeRef input, NodeRef pan)
    : input(input), pan(pan)
{
    this->name = "linear-panner";

    this->set_channels(1, channels);

    this->create_input("input", this->input);
    this->create_input("pan", this->pan);
}

void LinearPanner::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample pan = this->pan->out[0][frame];
        sample in = this->input->out[0][frame];

        if (pan < -1)
            pan = -1.0f;
        if (pan > 1)
            pan = 1.0;

        if (this->num_output_channels == 2)
        {
            pan = pan * 0.5 + 0.5;
            out[0][frame] = in * sqrtf((1.0 - pan));
            out[1][frame] = in * sqrtf(pan);
        }
        else
        {
            throw std::runtime_error("LinearPanner: Unsupported channel count");
        }
    }
}

}
