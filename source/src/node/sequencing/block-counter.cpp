#include "signalflow/node/sequencing/block-counter.h"

#include <limits>

namespace signalflow
{

BlockCounter::BlockCounter()
    : Node()
{
    this->name = "block-counter";
    this->counter = 0;
}

void BlockCounter::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = this->counter;
        }
    }
    this->counter += 1;
}

}
