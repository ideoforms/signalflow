#pragma once

#include "signal/node/fft/fftnode.h"
#include <vector>

namespace libsignal
{
class FFTConvolve : public FFTOpNode
{
public:
    FFTConvolve(NodeRef input = nullptr, BufferRef buffer = nullptr);
    virtual ~FFTConvolve();

    virtual void process(sample **out, int num_frames);

    BufferRef buffer;
    int num_partitions;
    std::vector <sample *> ir_partitions;
    std::vector <sample *> input_history;

};

REGISTER(FFTConvolve, "fft-convolve")
}
