#pragma once

#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{
class SawLFO : public LFO
{
public:
    SawLFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0);
    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(SawLFO, "saw-lfo")
}
