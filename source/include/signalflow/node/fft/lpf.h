#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT-based brick wall low pass filter.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTLPF : public FFTOpNode
{
public:
    FFTLPF(NodeRef input = 0, NodeRef frequency = 2000);
    virtual void process(Buffer &out, int num_frames);

    NodeRef frequency;
};

REGISTER(FFTLPF, "fft-lpf")
}
