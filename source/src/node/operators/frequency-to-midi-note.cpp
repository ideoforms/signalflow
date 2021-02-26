#include "signalflow/core/util.h"
#include "signalflow/node/operators/frequency-to-midi-note.h"

namespace signalflow
{

FrequencyToMidiNote::FrequencyToMidiNote(NodeRef a)
    : UnaryOpNode(a)
{
    this->name = "frequency-to-midi-note";
}

void FrequencyToMidiNote::process(Buffer &out, int num_frames)
{
    for (int channel = 0; channel < this->num_output_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            float value = this->input->out[channel][frame];
            float midi = signalflow_frequency_to_midi_note(value);
            out[channel][frame] = midi;
        }
    }
}

}
