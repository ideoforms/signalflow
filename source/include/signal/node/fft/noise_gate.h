#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{
class FFTNoiseGate : public UnaryOpNode
{
public:
    FFTNoiseGate(NodeRef input = 0, NodeRef threshold = 0.5);
    virtual void process(sample **out, int num_frames);

};

REGISTER(FFTNoiseGate, "fft_noise_gate")
}
