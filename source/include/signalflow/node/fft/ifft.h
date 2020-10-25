#pragma once

#include "signalflow/node/fft/fftnode.h"

#if defined(FFT_ACCELERATE)
#include <Accelerate/Accelerate.h>
#elif defined(FFT_FFTW)
#include <fftw3.h>
#endif

namespace signalflow
{
class IFFT : public FFTOpNode
{
public:
    IFFT(NodeRef input = nullptr, bool do_window = false);

    ~IFFT();

    sample *buffer;
#if defined(FFT_ACCELERATE)
    int log2N;
    FFTSetup fft_setup;
    sample *buffer2;
#elif defined(FFT_FFTW)
    fftwf_complex *fftw_buffer;
#endif

    sample *window;
    bool do_window;

    virtual void ifft(sample *in,
                      sample *out,
                      bool polar = true,
                      bool do_window = false,
                      float scale_factor = 1.0);
    virtual void process(Buffer &out, int num_frames);
};

REGISTER(IFFT, "ifft")
}
