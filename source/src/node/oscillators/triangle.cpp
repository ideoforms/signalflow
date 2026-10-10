#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/triangle.h"

namespace signalflow
{

TriangleOscillator::TriangleOscillator(NodeRef frequency)
    : frequency(frequency)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "triangle";
    this->create_input("frequency", this->frequency);
    this->alloc();
}

void TriangleOscillator::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void TriangleOscillator::process(Buffer &out, int num_frames)
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

        for (int frame = 0; frame < num_frames; frame++)
        {
            float rv = (phase < 0.5) ? (phase * 4.0 - 1.0) : (1.0 - (phase - 0.5) * 4.0);

            out_channel[frame] = rv;

            phase += frequency_channel[frame] / sample_rate;
            while (phase >= 1.0)
                phase -= 1.0;
        }
        this->phase[channel] = phase;
    }
}

}
