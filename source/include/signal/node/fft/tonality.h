#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{

class FFTTonality : public FFTOpNode
{
public:
    FFTTonality(NodeRef input = 0, NodeRef level = 0.5, NodeRef smoothing = 0.9);
    virtual void process(sample **out, int num_frames);

    NodeRef level = nullptr;
    NodeRef smoothing = nullptr;
};

REGISTER(FFTTonality, "fft_tonality")

}
