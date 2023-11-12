#include "signalflow/core/graph.h"
#include "signalflow/node/processors/filters/dc.h"

#include <stdlib.h>

namespace signalflow
{

DCFilter::DCFilter(NodeRef input)
    : UnaryOpNode(input)
{
    this->name = "dc-filter";

    // "R" depends on sampling rate and the low frequency point. Do not set "R" to a fixed value
    // (e.g. 0.99) if you don't know the sample rate. Instead set R to:
    // (-3dB @ 40Hz): R = 1-(250/samplerate)
    // (-3dB @ 30Hz): R = 1-(190/samplerate)
    // (-3dB @ 20Hz): R = 1-(126/samplerate)
    // this->R = 1.0 - (30.0 / this->get_graph()->get_sample_rate());

    this->alloc();
}

void DCFilter::alloc()
{
    this->previous_input.resize(this->num_output_channels_allocated);
    this->previous_output.resize(this->num_output_channels_allocated);
}

void DCFilter::process(Buffer &out, int num_frames)
{
    this->R = 1.0 - (30.0 / this->graph->get_sample_rate());
    for (int channel = 0; channel < num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float output = this->input->out[channel][frame] - this->previous_input[channel] + R * this->previous_output[channel];
            this->previous_input[channel] = this->input->out[channel][frame];
            this->previous_output[channel] = output;
            out[channel][frame] = output;
        }
    }
}

}
