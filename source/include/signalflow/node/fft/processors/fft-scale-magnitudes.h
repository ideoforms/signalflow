#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Randomise phase values.
 *---------------------------------------------------------------------------------*/
class FFTScaleMagnitudes : public FFTOpNode
{
public:
    FFTScaleMagnitudes(NodeRef input = 0, std::vector<float> scale = {});
    virtual void process(Buffer &out, int num_frames);

protected:
    std::vector<float> scale;
};

REGISTER(FFTScaleMagnitudes, "fft-scale-magnitudes")

}
