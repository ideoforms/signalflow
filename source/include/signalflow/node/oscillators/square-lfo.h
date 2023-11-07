#pragma once

#include "signalflow/node/oscillators/lfo.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a pulse wave LFO with the given frequency and pulse width, ranging from
 * min to max, where width=0.5 is a square wave.
 *---------------------------------------------------------------------------------*/
class SquareLFO : public LFO
{
public:
    SquareLFO(NodeRef frequency = 1.0, NodeRef min = 0.0, NodeRef max = 1.0, NodeRef width = 0.5, NodeRef phase = 0.0);
    virtual void process(Buffer &out, int num_frames) override;

protected:
    NodeRef width;
};

REGISTER(SquareLFO, "square-lfo")
}
