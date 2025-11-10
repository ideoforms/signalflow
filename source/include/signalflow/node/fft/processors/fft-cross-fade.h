#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * FFT FFTCrossFade.
 * Requires two FFT* inputs.
 *---------------------------------------------------------------------------------*/
class FFTCrossFade : public FFTOpNode
{
public:
    FFTCrossFade(NodeRef inputA = 0, NodeRef inputB = 0, NodeRef crossfade = 0.0);
    virtual void process(Buffer &out, int num_frames);

private:
    NodeRef inputB;
    NodeRef crossfade;
};

REGISTER(FFTCrossFade, "fft-cross-fade")

}
