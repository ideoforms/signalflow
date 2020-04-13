#include "signal/filters/resample.h"

#include "signal/oscillators/constant.h"
#include "signal/graph.h"

#include <stdlib.h>

namespace libsignal
{

Resample::Resample(NodeRef input, NodeRef sample_rate, NodeRef bit_rate) :
    UnaryOpNode(input), sample_rate(sample_rate), bit_rate(bit_rate)
{
    phase = 0.0;
    phase_last = -1.0;
    memset(sample_last, 0, sizeof(sample_last));

    this->name = "resample";
    this->add_input("sample_rate", this->sample_rate);
    this->add_input("bit_rate", this->bit_rate);
}

void Resample::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        float phase_increment = this->sample_rate->out[0][frame] / this->graph->sample_rate;

        for (int channel = 0; channel < num_output_channels; channel++)
        {
            out[channel][frame] = sample_last[channel];
            if (int(phase) > int(phase_last))
                sample_last[channel] = this->input->out[channel][frame];
            phase_last = phase;
        }

        phase += phase_increment;
    }
}

}
