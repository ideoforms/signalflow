#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/util.h"

#define SIGNALFLOW_DEFAULT_WAVESHAPER_BUFFER_LENGTH 2048

namespace signalflow
{

/*-------------------------------------------------------------------------
 * A WaveShaperBuffer is a mono buffer with a fixed number of samples
 * that can be sampled at a position [-1,1] to give an amplitude mapping
 * value, equal to the result of the shaper's transfer function for
 * that value.
 *-----------------------------------------------------------------------*/
class WaveShaperBuffer : public Buffer
{
public:
    WaveShaperBuffer(int length = SIGNALFLOW_DEFAULT_WAVESHAPER_BUFFER_LENGTH);
    WaveShaperBuffer(const std::function<float(float)> f);

    /**------------------------------------------------------------------------
     * Perform a waveshaper x -> f(x) transform.
     *
     * @param input A given input sample, between [-1, 1]
     * @return A transformed sample value, between [-1, 1].
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
};

}
