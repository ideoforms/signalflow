#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/buffer/buffer2d.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class Wavetable : public Node
{
public:
    Wavetable(BufferRef buffer = nullptr,
              NodeRef frequency = 440,
              NodeRef phase = 0,
              NodeRef sync = 0);

    virtual void process(sample **out, int num_frames);

private:
    BufferRef buffer;
    NodeRef frequency;
    NodeRef phase;
    NodeRef sync;

    float current_phase[SIGNALFLOW_MAX_CHANNELS];
};

class Wavetable2D : public Node
{
public:
    Wavetable2D(BufferRef2D buffer = nullptr,
                NodeRef frequency = 440,
                NodeRef crossfade = 0.0,
                NodeRef sync = 0);

    virtual void process(sample **out, int num_frames);

private:
    BufferRef2D buffer;
    NodeRef frequency;
    NodeRef crossfade;
    NodeRef sync;
    float phase[SIGNALFLOW_MAX_CHANNELS];
};

REGISTER(Wavetable, "wavetable")
REGISTER(Wavetable2D, "wavetable2d")
}
