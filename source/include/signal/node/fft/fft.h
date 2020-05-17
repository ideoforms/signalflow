#pragma once

#include "signal/node/fft/fftnode.h"

#include <Accelerate/Accelerate.h>

namespace libsignal
{
class FFT : public FFTNode
{
public:
    FFT(NodeRef input = 0.0,
        int fft_size = SIGNAL_DEFAULT_FFT_SIZE,
        int hop_size = SIGNAL_DEFAULT_FFT_HOP_SIZE);
    ~FFT();

    virtual void process(sample **out, int num_frames);

    NodeRef input;

private:

    virtual void fft(sample *in,
                     sample *out,
                     bool polar = true,
                     bool do_window = true);

    int log2N;
    FFTSetup fft_setup;
    sample *buffer;
    sample *buffer2;
    sample *inbuf;
    int inbuf_size;
    sample *window;

    friend class FFTConvolve;
};

REGISTER(FFT, "fft")
}
