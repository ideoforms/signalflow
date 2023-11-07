#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Map a frequency to a MIDI note (where 440Hz = A4 = 69), with floating-point
 * output.
 *---------------------------------------------------------------------------------*/
class FrequencyToMidiNote : public UnaryOpNode
{

public:
    FrequencyToMidiNote(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(FrequencyToMidiNote, "frequency-to-midi-note")

}
