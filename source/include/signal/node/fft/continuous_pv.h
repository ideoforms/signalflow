#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{
class FFTContinuousPhaseVocoder : public FFTNode
{
public:
    FFTContinuousPhaseVocoder(NodeRef input = nullptr, float rate = 1.0);

    virtual void process(sample **out, int num_frames);

    sample *phase_buffer_last;

    sample *magnitude_buffer;
    sample *phase_buffer;
    sample *phase_deriv;

    NodeRef input;
    float rate;

};

REGISTER(FFTContinuousPhaseVocoder, "fft_continuous_phase_vocoder")
}
