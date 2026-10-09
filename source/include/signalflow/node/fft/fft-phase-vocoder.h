#pragma once

#include "signalflow/node/fft/fft-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Phase vocoder.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTPhaseVocoder : public FFTOpNode
{
public:
    FFTPhaseVocoder(NodeRef input = nullptr);

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1);
    virtual void process(Buffer &out, int num_frames);

    sample *magnitude_buffer;
    sample *phase_buffer;
    sample *phase_deriv;
    bool frozen;
    bool just_frozen;

    NodeRef clock = nullptr;
};

REGISTER(FFTPhaseVocoder, "fft_phase_vocoder")
}
