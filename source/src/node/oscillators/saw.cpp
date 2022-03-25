#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/saw.h"

namespace signalflow
{

SawOscillator::SawOscillator(NodeRef frequency, NodeRef phase)
    : frequency(frequency), phase(phase)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "saw";
    this->create_input("frequency", this->frequency);
    this->create_input("phase", this->phase);
    this->alloc();
}

void SawOscillator::alloc()
{
    this->phase_offset.resize(this->num_output_channels_allocated);
}

void SawOscillator::process(Buffer &out, int num_frames)
{
    float phase_cur;
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (this->phase)
            {
                phase_cur = fmodf(this->phase_offset[channel] + this->phase->out[channel][frame], 1.0);
            }
            else
            {
                phase_cur = this->phase_offset[channel];
            }
            float rv = (phase_cur * 2.0) - 1.0;

            out[channel][frame] = rv;

            this->phase_offset[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
            while (this->phase_offset[channel] >= 1.0)
            {
                this->phase_offset[channel] -= 1.0;
            }
        }
    }
}

}
