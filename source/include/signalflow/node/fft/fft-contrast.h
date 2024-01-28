#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT Contrast.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTContrast : public FFTOpNode
{
public:
    FFTContrast(NodeRef input = 0, NodeRef contrast = 1);
    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef contrast;
    float mags[SIGNALFLOW_MAX_FFT_SIZE / 2];
};

REGISTER(FFTContrast, "fft-contrast")

}
