#pragma once

/**-------------------------------------------------------------------------
 * @file envelope-buffer.h
 * @brief EnvelopeBuffer stores an amplitude envelope from 0..1.
 *
 *-----------------------------------------------------------------------*/

#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/util.h"

#define SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH 2048

#define SIGNALFLOW_BUFFER_HANNING "hanning"
#define SIGNALFLOW_BUFFER_LINEAR_DECAY "linear-decay"
#define SIGNALFLOW_BUFFER_TRIANGLE "triangle"

namespace signalflow
{
/**-------------------------------------------------------------------------
 * An EnvelopeBuffer is a mono buffer with a fixed number of samples,
 * which can be sampled at a position [0,1] to give an amplitude value.
 *-----------------------------------------------------------------------*/
class EnvelopeBuffer : public Buffer
{
public:
    EnvelopeBuffer(int length = SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH);

    /**------------------------------------------------------------------------
      * Initialise a buffer with the envelope `shape`.
      *
      * The list of supported buffer identifiers:
      *
      *   hanning: Hanning envelope
      *   triangle: Symmetrical linear ramp from 0..1..0
      *   linear-decay: Linear decay from 1..0
      *   rectangular: Flat "box car" envelope, with all samples == 1.0.
      *
      * @param name One of the recognised shapes above
      * @param num_frames Length of buffer
      *
      *------------------------------------------------------------------------*/
    EnvelopeBuffer(std::string shape, int num_frames = SIGNALFLOW_DEFAULT_ENVELOPE_BUFFER_LENGTH);
    EnvelopeBuffer(const std::function<float(float)> f);
    EnvelopeBuffer(std::vector<float> samples);

    /**------------------------------------------------------------------------
     * @param position An envelope position between [0, 1].
     * @return An envelope amplitude value, between [0, 1].
     *------------------------------------------------------------------------*/
    virtual double offset_to_frame(double offset) override;
    virtual double frame_to_offset(double frame) override;
};

}
