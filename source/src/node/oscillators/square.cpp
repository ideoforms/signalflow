#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/square.h"

namespace signalflow
{

SquareOscillator::SquareOscillator(NodeRef frequency, NodeRef width)
    : frequency(frequency), width(width)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "square";

    this->create_input("frequency", this->frequency);
    this->create_input("width", this->width);

    this->alloc();
}

void SquareOscillator::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void SquareOscillator::process(Buffer &out, int num_frames)
{
    float sample_rate = this->graph->get_sample_rate();

    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        /*--------------------------------------------------------------------------------
         * Keep phase in a local variable during the loop, to avoid a load and store
         * per sample (see SineOscillator).
         *--------------------------------------------------------------------------------*/
        float phase = this->phase[channel];
        sample *out_channel = out[channel];
        sample *frequency_channel = this->frequency->out[channel];
        sample *width_channel = this->width->out[channel];

        for (int frame = 0; frame < num_frames; frame++)
        {
            float frequency = frequency_channel[frame];
            float width = width_channel[frame];
            float rv = (phase < width) ? 1 : -1;

            out_channel[frame] = rv;

            phase += frequency / sample_rate;
            if (phase >= 1.0)
                phase -= 1.0;
        }
        this->phase[channel] = phase;
    }
}

}
