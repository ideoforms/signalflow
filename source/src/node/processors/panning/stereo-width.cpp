#include "signalflow/node/processors/panning/stereo-width.h"

namespace signalflow
{

StereoWidth::StereoWidth(NodeRef input, NodeRef width)
    : UnaryOpNode(input), width(width)
{
    this->name = "stereo-width";
    this->create_input("stereo-width", this->width);

    this->set_channels(2, 2);
}

void StereoWidth::process(Buffer &out, int num_frames)
{
    // TODO fix this
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample width = this->width->out[0][frame];
        sample left = this->input->out[0][frame];
        sample right = this->input->out[1][frame];

        left = (1 - width) * left + width * right;
        right = (1 - width) * right + width * left;

        out[0][frame] = left;
        out[1][frame] = right;
    }
}

}
