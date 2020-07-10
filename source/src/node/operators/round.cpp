#include "signalflow/node/operators/round.h"

namespace signalflow
{

RoundToScale::RoundToScale(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "round-to-scale";
}

void RoundToScale::process(sample **out, int num_frames)
{
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            float value = this->input->out[channel][frame];
            float midi = signal_frequency_to_midi_note(value);
            float midi_rounded = roundf(midi);
            float freq = signal_midi_note_to_frequency(midi_rounded);
            out[channel][frame] = freq;
        }
    }
}

}
