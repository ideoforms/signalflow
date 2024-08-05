#pragma once

#include "signalflow/node/fft/fftnode.h"

#if defined(FFT_ACCELERATE)
#include <Accelerate/Accelerate.h>
#elif defined(FFT_FFTW)
#include <fftw3.h>
#endif

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Inverse Fast Fourier Transform.
 * Requires an FFT* input, generates a time-domain output.
 *---------------------------------------------------------------------------------*/
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
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;
};

REGISTER(IFFT, "ifft")
}
