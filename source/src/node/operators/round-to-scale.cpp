#include "signalflow/node/operators/round-to-scale.h"

namespace signalflow
{

RoundToScale::RoundToScale(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "round-to-scale";
}

void RoundToScale::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            float midi = signalflow_frequency_to_midi_note(value);
            float midi_rounded = roundf(midi);
            float freq = signalflow_midi_note_to_frequency(midi_rounded);
            out[channel][frame] = freq;
        }
    }
}

}
