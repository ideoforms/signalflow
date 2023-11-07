#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Biquad filter.
 * filter_type can be 'low_pass', 'band_pass', 'high_pass', 'notch', 'peak',
 * 'low_shelf', 'high_shelf'.
 * Not recommended for real-time modulation; for this, use SVFilter.
 *---------------------------------------------------------------------------------*/
class BiquadFilter : public UnaryOpNode
{
public:
    BiquadFilter(NodeRef input = 0.0,
                 signalflow_filter_type_t filter_type = SIGNALFLOW_FILTER_TYPE_LOW_PASS,
                 NodeRef cutoff = 440,
                 NodeRef resonance = 0.0,
                 NodeRef peak_gain = 0.0);

    BiquadFilter(NodeRef input,
                 std::string filter_type,
                 NodeRef cutoff = 440,
                 NodeRef resonance = 0.0,
                 NodeRef peak_gain = 0.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    signalflow_filter_type_t filter_type;
    NodeRef cutoff;
    NodeRef resonance;
    NodeRef peak_gain;

    virtual void _recalculate();

    std::vector<float> a0, a1, a2, b1, b2, z1, z2;
};

REGISTER(BiquadFilter, "biquad-filter")

}
