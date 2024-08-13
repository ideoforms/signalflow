#pragma once

/**-------------------------------------------------------------------------
 * @file wavetable-buffer.h
 * @brief WavetableBuffer stores an amplitude envelope from -1..1.
 *
 *-----------------------------------------------------------------------*/

#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/util.h"

#define SIGNALFLOW_DEFAULT_WAVETABLE_BUFFER_LENGTH 2048

namespace signalflow
{
/**-------------------------------------------------------------------------
 * A WavetableBuffer is a mono buffer with a fixed number of samples,
 * which can be sampled at a position [0,1] to give a bipolar [-1, 1]
 * amplitude value, intended for use with the Wavetable node.
 *
 * At some point, it would be nice to support multi-sampled wavetables
 * band-limited per octave.
 *
 * Some useful notes:
 * https://www.earlevel.com/main/category/digital-audio/oscillators/wavetable-oscillators/
 *-----------------------------------------------------------------------*/
class WavetableBuffer : public Buffer
{
public:
    WavetableBuffer(int length = SIGNALFLOW_DEFAULT_WAVETABLE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
      * Initialise a buffer with the envelope `shape`.
      *
      * Supported buffer shapes:
      *
      *   sine: Sine wave
      *   triangle: Triangle wave
      *
      * @param name One of the recognised shapes above
      * @param num_frames Length of buffer
      *
      *------------------------------------------------------------------------*/
    WavetableBuffer(std::string shape, int num_frames = SIGNALFLOW_DEFAULT_WAVETABLE_BUFFER_LENGTH);
    WavetableBuffer(const std::function<float(float)> f);
    WavetableBuffer(std::vector<float> samples);

    /**------------------------------------------------------------------------
     * @param position An envelope position between [0, 1].
     * @return A frame position
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
};

}
