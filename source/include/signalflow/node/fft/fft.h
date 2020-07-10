#pragma once

#include "signalflow/node/fft/fftnode.h"

#if defined(FFT_ACCELERATE)
#include <Accelerate/Accelerate.h>
#elif defined(FFT_FFTW)
#include <fftw3.h>
#endif

namespace signalflow
{
class FFT : public FFTNode
{
public:
    FFT(NodeRef input = 0.0,
        int fft_size = SIGNAL_DEFAULT_FFT_SIZE,
        int hop_size = SIGNAL_DEFAULT_FFT_HOP_SIZE,
        int window_size = 0,
        bool do_window = true);
    ~FFT();

    virtual void process(sample **out, int num_frames);

    NodeRef input;

private:
    virtual void fft(sample *in,
                     sample *out,
                     bool polar = true,
                     bool do_window = true);

#if defined(FFT_ACCELERATE)
    FFTSetup fft_setup;
    sample *buffer;
    sample *buffer2;
    int log2N;
#elif defined(FFT_FFTW)
    sample *buffer;
    fftwf_complex *fftw_buffer;
#endif

    sample *window;
    sample *input_buffer;
    int input_buffer_size;

    friend class FFTConvolve;
};

REGISTER(FFT, "fft")
}
