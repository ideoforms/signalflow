#include "signalflow/node/filters/smooth.h"

namespace signalflow
{

Smooth::Smooth(NodeRef input, NodeRef smooth)
    : UnaryOpNode(input), smooth(smooth)
{
    this->name = "smooth";
    this->add_input("smooth", this->smooth);

    this->values.resize(SIGNAL_MAX_CHANNELS);
    this->values.clear();
}

void Smooth::process(sample **out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            values[channel] = (smooth->out[channel][frame] * values[channel]) + ((1.0 - smooth->out[channel][frame]) * input->out[channel][frame]);
            out[channel][frame] = values[channel];
        }
    }
}

}
