#include "signalflow/node/processors/dynamics/rms.h"

namespace signalflow
{

RMS::RMS(NodeRef input)
    : UnaryOpNode(input)
{
    this->name = "rms";
}

void RMS::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        double sum_squared = 0;
        for (int frame = 0; frame < num_frames; frame++)
        {
            sum_squared += input->out[channel][frame] * input->out[channel][frame];
        }
        double mean_squared = sum_squared / num_frames;
        double rms = sqrt(mean_squared);

        for (int frame = 0; frame < num_frames; frame++)
        {
            out[channel][frame] = rms;
        }
    }
}

}
