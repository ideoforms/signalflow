#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{
class FFTLPF : public FFTOpNode
{
public:
    FFTLPF(NodeRef input = 0, NodeRef frequency = 2000);
    virtual void process(sample **out, int num_frames);

    NodeRef frequency;

};

REGISTER(FFTLPF, "fft_lpf")
}
