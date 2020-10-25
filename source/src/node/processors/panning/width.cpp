#include "signalflow/node/processors/panning/width.h"

namespace signalflow
{

Width::Width(NodeRef input, NodeRef width)
    : UnaryOpNode(input), width(width)
{
    this->name = "width";
    this->create_input("width", this->width);

    this->set_channels(2, 2);
}

void Width::process(Buffer &out, int num_frames)
{
    // TODO fix this
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample width = this->width->out[0][frame];

        sample **in = this->input->out.data;
        sample left = in[0][frame];
        sample right = in[1][frame];

        left = (1 - width) * left + width * right;
        right = (1 - width) * right + width * left;

        out[0][frame] = left;
        out[1][frame] = right;
    }
}

}
