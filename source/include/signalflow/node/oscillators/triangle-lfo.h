#pragma once

#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a triangle LFO with the given frequency, ranging from min to max.
 *---------------------------------------------------------------------------------*/
class TriangleLFO : public LFO
{
public:
    TriangleLFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0, NodeRef phase = 0.0);
    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(TriangleLFO, "triangle-lfo")
}
