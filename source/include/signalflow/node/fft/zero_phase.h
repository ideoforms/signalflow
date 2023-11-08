#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Remove phase information from a frequency-domain input.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTZeroPhase : public UnaryOpNode
{
public:
    FFTZeroPhase(NodeRef input = 0);

    virtual void process(Buffer &out, int num_frames);
};

REGISTER(FFTZeroPhase, "zero_phase")
}
