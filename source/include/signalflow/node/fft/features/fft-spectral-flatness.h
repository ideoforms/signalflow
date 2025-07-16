#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Randomise phase values.
 *---------------------------------------------------------------------------------*/
class FFTSpectralFlatness : public FFTOpNode
{
public:
    FFTSpectralFlatness(NodeRef input = 0);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(FFTSpectralFlatness, "fft-spectral-flatness")

}
