#pragma once

#include "signal/node/fft/fftnode.h"

#include <Accelerate/Accelerate.h>

namespace libsignal
{
class IFFT : public FFTOpNode
{
public:
    IFFT(NodeRef input = nullptr);

    ~IFFT();

    int log2N;
    FFTSetup fft_setup;
    sample *buffer;
    sample *buffer2;
    sample *window;

    virtual void ifft(sample *in,
                      sample *out,
                      bool polar = true,
                      bool do_window = false,
                      float scale_factor = 1.0);
    virtual void process(sample **out, int num_frames);
};

REGISTER(IFFT, "ifft")
}
