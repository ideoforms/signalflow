#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Simulation of the Moog ladder low-pass filter. `cutoff` sets the cutoff
 * frequency; `resonance` should typically be between 0..1.
 *---------------------------------------------------------------------------------*/
class MoogVCF : public UnaryOpNode
{
public:
    MoogVCF(NodeRef input = 0.0, NodeRef cutoff = 200.0, NodeRef resonance = 0.0);

    NodeRef cutoff;
    NodeRef resonance;

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    std::vector<float> out1, out2, out3, out4;
    std::vector<float> in1, in2, in3, in4;
};

REGISTER(MoogVCF, "moog")
}
