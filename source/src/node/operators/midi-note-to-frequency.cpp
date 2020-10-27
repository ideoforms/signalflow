#include "signalflow/core/util.h"
#include "signalflow/node/operators/midi-note-to-frequency.h"

namespace signalflow
{

MidiNoteToFrequency::MidiNoteToFrequency(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "midi-note-to-frequency";
}

void MidiNoteToFrequency::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            float midi = signalflow_midi_note_to_frequency(value);
            out[channel][frame] = midi;
        }
    }
}

}
