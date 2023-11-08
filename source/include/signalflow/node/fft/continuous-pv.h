#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Continuous phase vocoder.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTContinuousPhaseVocoder : public FFTNode
{
public:
    FFTContinuousPhaseVocoder(NodeRef input = nullptr, float rate = 1.0);

    virtual void process(Buffer &out, int num_frames);
    virtual void trigger(std::string name, float value);

    sample *magnitude_buffer;
    sample *phase_buffer;
    sample *phase_deriv;

    NodeRef input;
    float rate;

    bool prefilled_fft_buffer;
};

REGISTER(FFTContinuousPhaseVocoder, "fft-continuous-pv")
}
