#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/saw.h"

namespace signalflow
{

SawOscillator::SawOscillator(NodeRef frequency, NodeRef phase_offset, NodeRef reset)
    : frequency(frequency), phase_offset(phase_offset), reset(reset)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "saw";
    this->create_input("frequency", this->frequency);
    this->create_input("phase_offset", this->phase_offset);
    this->create_input("reset", this->reset);
    this->alloc();
}

void SawOscillator::alloc()
{
    this->phase.resize(this->num_output_channels_allocated);
}

void SawOscillator::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->phase[channel] = 0;
        }
    }
}

void SawOscillator::process(Buffer &out, int num_frames)
{
    float phase_cur;
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            if (this->reset)
            {
                if (this->reset->out[channel][frame])
                {
                    this->phase[channel] = 0;
                }
            }

            if (this->phase_offset)
            {
                phase_cur = fmodf(this->phase[channel] + this->phase_offset->out[channel][frame], 1.0);
            }
            else
            {
                phase_cur = this->phase[channel];
            }
            float rv = (phase_cur * 2.0) - 1.0;

            out[channel][frame] = rv;

            this->phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
            while (this->phase[channel] >= 1.0)
            {
                this->phase[channel] -= 1.0;
            }
        }
    }
}

}
