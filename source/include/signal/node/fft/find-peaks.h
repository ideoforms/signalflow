#pragma once

#include "signal/node/fft/fftnode.h"

namespace libsignal
{

class FFTFindPeaks : public FFTOpNode
{
public:
    FFTFindPeaks(NodeRef input = 0, int count = 8,
                 NodeRef prominence = 1, NodeRef threshold = 1e-6);
    virtual void process(sample **out, int num_frames);

    NodeRef prominence = nullptr;
    NodeRef threshold = nullptr;
    int count;
};

REGISTER(FFTFindPeaks, "fft-find-peaks")

}
