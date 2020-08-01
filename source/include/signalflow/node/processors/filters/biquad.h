#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

typedef enum
{
    SIGNALFLOW_FILTER_TYPE_LOW_PASS,
    SIGNALFLOW_FILTER_TYPE_HIGH_PASS,
    SIGNALFLOW_FILTER_TYPE_BAND_PASS,
    SIGNALFLOW_FILTER_TYPE_NOTCH,
    SIGNALFLOW_FILTER_TYPE_PEAK,
    SIGNALFLOW_FILTER_TYPE_LOW_SHELF,
    SIGNALFLOW_FILTER_TYPE_HIGH_SHELF
} signalflow_filter_type_t;

namespace signalflow
{
class BiquadFilter : public UnaryOpNode
{
public:
    BiquadFilter(NodeRef input = 0.0,
                 signalflow_filter_type_t filter_type = SIGNALFLOW_FILTER_TYPE_LOW_PASS,
                 NodeRef cutoff = 440,
                 NodeRef resonance = 0.707,
                 NodeRef peak_gain = 0.0);

    signalflow_filter_type_t filter_type;
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
