#pragma once

#include "signal/node/fft/fftnode.h"

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#include <fftw3.h>
#endif

namespace libsignal
{
class IFFT : public FFTOpNode
{
public:
    IFFT(NodeRef input = nullptr, bool do_window = false);

    ~IFFT();

#ifdef __APPLE__
    int log2N;
    FFTSetup fft_setup;
    sample *buffer;
    sample *buffer2;
#else
    fftwf_complex *fftw_buffer;
#endif

    sample *window;
    bool do_window;

    virtual void ifft(sample *in,
                      sample *out,
                      bool polar = true,
                      bool do_window = false,
                      float scale_factor = 1.0);
    virtual void process(sample **out, int num_frames);
};

REGISTER(IFFT, "ifft")
}
