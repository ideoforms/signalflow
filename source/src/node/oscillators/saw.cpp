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
    float sample_rate = this->graph->get_sample_rate();
    Node *reset = this->reset.get();
    Node *phase_offset = this->phase_offset.get();

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
            if (reset)
            {
                if (reset->out[channel][frame])
                {
                    phase = 0;
                }
            }

            if (phase_offset)
            {
                phase_cur = fmodf(phase + phase_offset->out[channel][frame], 1.0);
            }
            else
            {
                phase_cur = phase;
            }
            float rv = (phase_cur * 2.0) - 1.0;

            out_channel[frame] = rv;

            phase += frequency_channel[frame] / sample_rate;
            while (phase >= 1.0)
            {
                phase -= 1.0;
            }
        }
        this->phase[channel] = phase;
    }
}

}
