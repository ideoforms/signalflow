#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT LFO.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTLFO : public FFTOpNode
{
public:
    FFTLFO(NodeRef input = 0, NodeRef frequency = 1.0, NodeRef spectral_cycles = 1.0);
    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef frequency;
    NodeRef spectral_cycles;
    double phase;
};

REGISTER(FFTLFO, "fft-lfo")

}
