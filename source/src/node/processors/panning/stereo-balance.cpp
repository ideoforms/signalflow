#include "signalflow/node/processors/panning/stereo-balance.h"

namespace signalflow
{

StereoBalance::StereoBalance(NodeRef input, NodeRef balance)
    : UnaryOpNode(input), balance(balance)
{
    this->name = "stereo-balance";
    this->create_input("stereo-balance", this->balance);
    this->set_channels(2, 2);
}

void StereoBalance::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        sample balance = this->balance->out[0][frame];
        sample left = this->input->out[0][frame];
        sample right = this->input->out[1][frame];

        balance = 0.5 + (0.5 * balance);
        left = left * sqrtf(1 - balance);
        right = right * sqrtf(balance);

        out[0][frame] = left;
        out[1][frame] = right;
    }
}

}
