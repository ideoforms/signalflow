#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a pulse wave with the given frequency and pulse width, where width=0.5 is
 * a square wave.
 *---------------------------------------------------------------------------------*/
class SquareOscillator : public Node
{
public:
    SquareOscillator(NodeRef frequency = 440, NodeRef width = 0.5);

    NodeRef frequency;
    NodeRef width;

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    std::vector<float> phase;
};

REGISTER(SquareOscillator, "square")
}
