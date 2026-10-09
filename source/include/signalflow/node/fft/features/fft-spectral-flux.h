#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Spectral flux: measures the change in magnitude spectrum between frames.
 *---------------------------------------------------------------------------------*/
class FFTSpectralFlux : public FFTOpNode
{
public:
    FFTSpectralFlux(NodeRef input = 0);
    virtual void process(Buffer &out, int num_frames) override;
    virtual void alloc() override;

private:
    std::vector<float> previous_magnitudes;
};

REGISTER(FFTSpectralFlux, "fft-spectral-flux")

}
