#include "signal/filters/width.h"

namespace libsignal
{

Width::Width(NodeRef input, NodeRef width) : UnaryOpNode(input), width(width)
{
    this->name = "width";
    this->add_input("width", this->width);

    this->num_input_channels = 2;
    this->num_output_channels = 2;
}

void Width::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample width = this->width->out[0][frame];

        sample **in = this->input->out;
        sample left = in[0][frame];
        sample right = in[1][frame];

        left = (1 - width) * left + width * right;
        right = (1 - width) * right + width * left;

        out[0][frame] = left;
        out[1][frame] = right;
    }
}

}
