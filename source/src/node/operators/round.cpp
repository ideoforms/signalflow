#include "signalflow/node/operators/round.h"
#include <math.h>

namespace signalflow
{

Round::Round(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "round";
}

void Round::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            out[channel][frame] = roundf(value);
        }
    }
}

}
