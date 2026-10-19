#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs the value of `buffer` at the frame given by `index`, for each of the
 * buffer's channels. `index` is in frames and is clamped to the buffer's bounds.
 * Fractional values are interpolated according to the buffer's interpolation_mode.
 * Useful for reading a table of values (e.g. a step sequence, or onset positions).
 *---------------------------------------------------------------------------------*/
class BufferLookup : public Node
{
public:
    BufferLookup(BufferRef buffer = nullptr, NodeRef index = 0);

    virtual void process(Buffer &out, int num_frames);
    virtual void set_buffer(std::string name, BufferRef buffer);

    BufferRef buffer;

    NodeRef index;
};

REGISTER(BufferLookup, "buffer-lookup")
}
