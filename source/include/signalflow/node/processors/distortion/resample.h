#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Resampler and bit crusher. sample_rate is in Hz, bit_rate is an integer
 * between 0 and 16.
 *---------------------------------------------------------------------------------*/
class Resample : public UnaryOpNode
{
public:
    Resample(NodeRef input = 0, NodeRef sample_rate = 44100, NodeRef bit_rate = 16);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef sample_rate;
    NodeRef bit_rate;

    float phase;
    float phase_last;
    std::vector<sample> sample_last;
};

REGISTER(Resample, "resample")
}
