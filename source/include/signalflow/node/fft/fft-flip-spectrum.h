#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Flips the FFT magnitude spectrum in the X axis.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTFlipSpectrum : public FFTOpNode
{
public:
    FFTFlipSpectrum(NodeRef input = 0, NodeRef flip = 0, NodeRef rotate = 0);
    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef flip;
    NodeRef rotate;
};

REGISTER(FFTFlipSpectrum, "fft-flip-spectrum")

}
