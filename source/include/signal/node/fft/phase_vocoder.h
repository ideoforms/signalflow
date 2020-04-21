#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{
class FFTPhaseVocoder : public FFTOpNode
{
public:
    FFTPhaseVocoder(NodeRef input = nullptr);

    virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1);
    virtual void process(sample **out, int num_frames);

    sample *magnitude_buffer;
    sample *phase_buffer;
    sample *phase_deriv;
    bool frozen;
    bool just_frozen;

    NodeRef clock = nullptr;

};

REGISTER(FFTPhaseVocoder, "fft_phase_vocoder");
}
