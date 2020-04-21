#pragma once

/**-------------------------------------------------------------------------
 * @file buffer2D.h
 * @brief Interpolating 2D buffers
 *-----------------------------------------------------------------------*/
#include "signal/buffer/buffer.h"

#include <vector>

namespace libsignal
{

class Buffer2D : public Buffer
{
public:
    Buffer2D(BufferRef bufferA, BufferRef bufferB);

    /**------------------------------------------------------------------------
     * Query a sample in the 2D buffer space.
     *
     * @param offset_x Normalised position in waveform, between [0, 1]
     * @param crossfade Fade between bufferA and bufferB, between [0, 1]
     * @return A transformed sample value, between [-1, 1].
     *------------------------------------------------------------------------*/
    sample get2D(double offset_x, double offset_z);

private:
    BufferRef bufferA;
    BufferRef bufferB;
};

typedef BufferRefTemplate<Buffer2D> BufferRef2D;

}
