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
              NodeRef sync = 0,
              BufferRef phase_map = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    BufferRef buffer;
    NodeRef frequency;
    NodeRef phase;
    NodeRef sync;
    BufferRef phase_map;

    std::vector<float> current_phase;
};

class Wavetable2D : public Node
{
public:
    // TODO: Implement phase argument
    Wavetable2D(BufferRef2D buffer = nullptr,
                NodeRef frequency = 440,
                NodeRef crossfade = 0.0,
                NodeRef phase = 0.0,
                NodeRef sync = 0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    BufferRef2D buffer;
    NodeRef frequency;
    NodeRef crossfade;
    NodeRef phase;
    NodeRef sync;

    std::vector<float> phase_offset;
};

REGISTER(Wavetable, "wavetable")
REGISTER(Wavetable2D, "wavetable2d")
}
