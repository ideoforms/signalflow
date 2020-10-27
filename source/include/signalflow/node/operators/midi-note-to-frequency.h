#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class MidiNoteToFrequency : public UnaryOpNode
{

public:
    MidiNoteToFrequency(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(MidiNoteToFrequency, "midi-note-to-frequency")

}
