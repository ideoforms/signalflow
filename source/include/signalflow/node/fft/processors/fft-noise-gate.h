#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT-based noise gate.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTNoiseGate : public FFTOpNode
{
public:
    FFTNoiseGate(NodeRef input = 0, NodeRef threshold = 0.5, NodeRef invert = 0.0);
    virtual void process(Buffer &out, int num_frames);

    NodeRef threshold = nullptr;
    NodeRef invert = nullptr;

private:
    float mags[SIGNALFLOW_MAX_FFT_SIZE / 2];
};

REGISTER(FFTNoiseGate, "fft_noise_gate")

}
