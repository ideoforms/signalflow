#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/saw-lfo.h"

namespace signalflow
{

SawLFO::SawLFO(NodeRef frequency, NodeRef min, NodeRef max, NodeRef phase)
    : LFO(frequency, min, max, phase)
{
    this->name = "saw-lfo";
}

void SawLFO::trigger(std::string name, float value)
{
    if (name == SIGNALFLOW_DEFAULT_TRIGGER)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            this->current_phase[channel] = 0;
        }
    }
}

void SawLFO::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float min = this->min->out[channel][frame];
            float max = this->max->out[channel][frame];
            float phase = fmod(this->phase->out[channel][frame] + this->current_phase[channel], 1);
            out[channel][frame] = min + phase * (max - min);

            this->current_phase[channel] += this->frequency->out[channel][frame] / this->graph->get_sample_rate();
        }
        while (this->current_phase[channel] >= 1.0)
            this->current_phase[channel] -= 1.0;
    }
}

}
