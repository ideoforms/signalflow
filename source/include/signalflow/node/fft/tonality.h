#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

class FFTTonality : public FFTOpNode
{
public:
    FFTTonality(NodeRef input = 0, NodeRef level = 0.5, NodeRef smoothing = 0.9);
    virtual void process(Buffer &out, int num_frames);

    NodeRef level = nullptr;
    NodeRef smoothing = nullptr;

private:
    float mags_smoothed[SIGNALFLOW_MAX_FFT_SIZE / 2];
};

REGISTER(FFTTonality, "fft-tonality")

}
