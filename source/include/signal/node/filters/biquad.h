#pragma once

#include "signal/core/constants.h"
#include "signal/node/node.h"

typedef enum
{
    SIGNAL_FILTER_TYPE_LOW_PASS,
    SIGNAL_FILTER_TYPE_HIGH_PASS,
    SIGNAL_FILTER_TYPE_BAND_PASS,
    SIGNAL_FILTER_TYPE_NOTCH,
    SIGNAL_FILTER_TYPE_PEAK,
    SIGNAL_FILTER_TYPE_LOW_SHELF,
    SIGNAL_FILTER_TYPE_HIGH_SHELF
} signal_filter_type_t;

namespace libsignal
{
class BiquadFilter : public UnaryOpNode
{
public:
    BiquadFilter(NodeRef input = 0.0,
                 signal_filter_type_t filter_type = SIGNAL_FILTER_TYPE_LOW_PASS,
                 NodeRef cutoff = 440,
                 NodeRef resonance = 0.707,
                 NodeRef peak_gain = 0.0);

    signal_filter_type_t filter_type;
    NodeRef cutoff;
    NodeRef resonance;
    NodeRef peak_gain;

    virtual void process(sample **out, int num_frames);

private:
    virtual void _recalculate();

    float a0, a1, a2, b1, b2;
    float z1, z2;
};

REGISTER(BiquadFilter, "biquad-filter")

}
