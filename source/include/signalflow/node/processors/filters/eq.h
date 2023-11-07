#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Three-band EQ.
 *---------------------------------------------------------------------------------*/
class EQ : public UnaryOpNode
{
public:
    EQ(NodeRef input = 0.0, NodeRef low_gain = 1.0, NodeRef mid_gain = 1.0, NodeRef high_gain = 1.0,
       NodeRef low_freq = 500, NodeRef high_freq = 5000);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef low_gain;
    NodeRef mid_gain;
    NodeRef high_gain;
    NodeRef low_freq;
    NodeRef high_freq;

private:
    std::vector<float> f1p0, f1p1, f1p2, f1p3;
    std::vector<float> f2p0, f2p1, f2p2, f2p3;
    std::vector<float> sdm1, sdm2, sdm3;
};

REGISTER(EQ, "eq")
}
