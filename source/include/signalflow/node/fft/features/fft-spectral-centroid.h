#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Randomise phase values.
 *---------------------------------------------------------------------------------*/
class FFTSpectralCentroid : public FFTOpNode
{
public:
    FFTSpectralCentroid(NodeRef input = 0);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(FFTSpectralCentroid, "fft-spectral-centroid")

}
