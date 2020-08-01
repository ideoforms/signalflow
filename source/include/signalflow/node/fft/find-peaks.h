#pragma once

#include "signalflow/node/fft/fftnode.h"

namespace signalflow
{

class FFTFindPeaks : public FFTOpNode
{
public:
    FFTFindPeaks(NodeRef input = 0,
                 NodeRef prominence = 1, NodeRef threshold = 0.000001,
                 int count = SIGNALFLOW_MAX_CHANNELS, bool interpolate = true);
    virtual void process(sample **out, int num_frames);

    NodeRef prominence = nullptr;
    NodeRef threshold = nullptr;
    int count;
    bool interpolate;
};

REGISTER(FFTFindPeaks, "fft-find-peaks")

}
