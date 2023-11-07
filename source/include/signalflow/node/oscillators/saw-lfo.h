#pragma once

#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a sawtooth LFO, with output ranging from min to max.
 *---------------------------------------------------------------------------------*/
class SawLFO : public LFO
{
public:
    SawLFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0, NodeRef phase = 0.0);
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
};

REGISTER(SawLFO, "saw-lfo")
}
