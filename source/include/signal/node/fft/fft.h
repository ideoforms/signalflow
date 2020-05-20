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
        int hop_size = SIGNAL_DEFAULT_FFT_HOP_SIZE,
        bool do_window = true);
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
    sample *input_buffer;
    int input_buffer_size;
    sample *window;
    bool do_window;

    friend class FFTConvolve;
};

REGISTER(FFT, "fft")
}
