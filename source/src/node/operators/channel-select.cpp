#include "signal/node/operators/channel-select.h"

namespace libsignal
{

ChannelSelect::ChannelSelect(NodeRef input, int offset, int maximum, int step)
    : UnaryOpNode(input), offset(offset), maximum(maximum ? maximum : offset + 1), step(step)
{
    this->name = "channel-select";

    this->num_output_channels = (int) ceilf((offset + maximum) / step);
}

void ChannelSelect::process(sample **out, int num_frames)
{
    int output_channel = 0;
    for (int channel = this->offset; channel < this->maximum; channel += this->step)
    {
        memcpy(out[output_channel], input->out[channel], num_frames * sizeof(sample));
        output_channel++;
    }
}

}
