#pragma once

#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{
class SineLFO : public LFO
{
public:
    SineLFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0, NodeRef phase = 0.0);
    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(SineLFO, "sine-lfo")
}
