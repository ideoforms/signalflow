#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a triangle wave with the given frequency.
 *---------------------------------------------------------------------------------*/

class TriangleOscillator : public Node
{
public:
    TriangleOscillator(NodeRef frequency = 440);

    NodeRef frequency;

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    std::vector<float> phase;
};

REGISTER(TriangleOscillator, "triangle")
}
