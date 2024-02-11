#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Fixed mag/phase array.
 *---------------------------------------------------------------------------------*/
class FFTMagnitudePhaseArray : public FFTOpNode
{
public:
    FFTMagnitudePhaseArray(NodeRef input = 0,
                           std::vector<float> magnitudes = {},
                           std::vector<float> phases = {});
    virtual void process(Buffer &out, int num_frames);

    void set_magnitudes(std::vector<float> &magnitudes);
    void set_phases(std::vector<float> &phases);
};

REGISTER(FFTMagnitudePhaseArray, "fft-magnitude-phase-array")

}
