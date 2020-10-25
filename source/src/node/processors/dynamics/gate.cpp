#include "signalflow/node/processors/dynamics/gate.h"

namespace signalflow
{

Gate::Gate(NodeRef input, NodeRef threshold)
    : UnaryOpNode(input), threshold(threshold)
{
    this->level = 0.0;

    this->name = "gate";
    this->create_input("threshold", this->threshold);
}

void Gate::process(Buffer &out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float amp = 0.0;
        for (int channel = 0; channel < this->num_input_channels; channel++)
            amp += this->input->out[channel][frame];
        amp /= this->num_input_channels;

        float threshold = this->threshold->out[0][frame];

        if (amp > threshold)
        {
            level = 1.0;
        }
        else
        {
            level = 0.0;
        }

        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = this->input->out[channel][frame] * level;
        }
    }
}

}
