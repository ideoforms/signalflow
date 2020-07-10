#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{
class FFTZeroPhase : public UnaryOpNode
{
public:
    FFTZeroPhase(NodeRef input = 0);

    virtual void process(sample **out, int num_frames);
};

REGISTER(FFTZeroPhase, "zero_phase")
}
