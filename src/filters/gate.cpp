#include "signal/filters/gate.h"

namespace libsignal
{

Gate::Gate(NodeRef input, NodeRef threshold) :
    UnaryOpNode(input), threshold(threshold)
{
    this->level = 0.0;

    this->name = "gate";
    this->add_input("threshold", this->threshold);
}

void Gate::process(sample **out, int num_frames)
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
            this->out[channel][frame] = this->input->out[channel][frame] * level;
        }
    }
}

}
