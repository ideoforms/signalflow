#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT-based noise gate.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTNoiseGate : public FFTOpNode
{
public:
    FFTNoiseGate(NodeRef input = 0, NodeRef threshold = 0.5);
    virtual void process(Buffer &out, int num_frames);

    NodeRef threshold = nullptr;

private:
    float mags[SIGNALFLOW_MAX_FFT_SIZE / 2];
};

REGISTER(FFTNoiseGate, "fft_noise_gate")

}
