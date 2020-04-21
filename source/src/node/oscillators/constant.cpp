#include "signal/node/oscillators/constant.h"

namespace libsignal
{

Constant::Constant(sample value)
    : Node()
{
    this->value = value;
    this->name = "constant";

    this->min_input_channels = 0;
    this->max_input_channels = 0;
}

void Constant::process(sample **out, int num_frames)
{
#if __APPLE__
    vDSP_vfill(&(this->value), out[0], 1, num_frames);
#else
    for (int frame = 0; frame < num_frames; frame++)
    {
        out[0][frame] = this->value;
    }
#endif
}

}
