#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{
class FFTConvolve : public FFTOpNode
{
public:
    FFTConvolve(NodeRef input = nullptr, BufferRef buffer = nullptr);

    virtual void process(sample **out, int num_frames);

    BufferRef buffer;
    sample *fft_buffer;

};

REGISTER(FFTConvolve, "fft-convolve")
}
