#include "signalflow/node/oscillators/constant.h"

namespace signalflow
{

Constant::Constant(sample value)
    : Node()
{
    this->value = value;
    this->name = "constant";
    this->matches_input_channels = false;
    this->num_input_channels = 0;
    this->num_output_channels = 1;
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
