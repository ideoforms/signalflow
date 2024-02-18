#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/buffer/buffer2d.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Plays the wavetable stored in buffer at the given `frequency` and `phase` offset.
 * `sync` can be used to provide a hard sync input, which resets the wavetable's
 * phase at each zero-crossing.
 *---------------------------------------------------------------------------------*/
class Wavetable : public Node
{
public:
    Wavetable(BufferRef buffer = nullptr,
              NodeRef frequency = 440,
              NodeRef phase_offset = 0,
              NodeRef sync = 0,
              BufferRef phase_map = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    BufferRef buffer;
    NodeRef frequency;
    NodeRef phase_offset;
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
                NodeRef phase_offset = 0.0,
                NodeRef sync = 0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    BufferRef2D buffer;
    NodeRef frequency;
    NodeRef crossfade;
    NodeRef phase_offset;
    NodeRef sync;

    std::vector<float> current_phase;
};

REGISTER(Wavetable, "wavetable")
REGISTER(Wavetable2D, "wavetable2d")
}
