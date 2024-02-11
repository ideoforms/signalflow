#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Randomise phase values.
 *---------------------------------------------------------------------------------*/
class FFTRandomPhase : public FFTOpNode
{
public:
    FFTRandomPhase(NodeRef input = 0, NodeRef level = 1.0);
    virtual void process(Buffer &out, int num_frames);

protected:
    NodeRef level;
};

REGISTER(FFTRandomPhase, "fft-random-phase")

}
