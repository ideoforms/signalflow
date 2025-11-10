#pragma once

#include "signalflow/node/fft/fft-node.h"
#include <vector>

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Frequency-domain convolution, using overlap-add.
 * Useful for convolution reverb, with the input buffer containing an impulse response.
 * Requires an FFT* input.
 *---------------------------------------------------------------------------------*/
class FFTConvolve : public FFTOpNode
{
public:
    FFTConvolve(NodeRef input = nullptr, BufferRef buffer = nullptr);
    virtual ~FFTConvolve();

    virtual void process(Buffer &out, int num_frames);

private:
    BufferRef buffer;
    int num_partitions;
    std::vector<sample *> ir_partitions;
    std::vector<sample *> input_history;
    sample *output_partition_polar_split;
    sample *output_partition_polar;
    sample *output_partition_cartesian;
    sample *output_sum_cartesian;
    sample *output_sum_polar;
};

REGISTER(FFTConvolve, "fft-convolve")
}
