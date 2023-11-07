#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Map a MIDI note to a frequency (where 440Hz = A4 = 69), supporting floating-point
 * input.
 *---------------------------------------------------------------------------------*/
class MidiNoteToFrequency : public UnaryOpNode
{

public:
    MidiNoteToFrequency(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(MidiNoteToFrequency, "midi-note-to-frequency")

}
