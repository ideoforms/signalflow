#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Produces a value of 1 at the given `frequency`, with output of 0 at all other times.
 * If frequency is 0, produces a single impulse.
 *---------------------------------------------------------------------------------*/
class Impulse : public Node
{
public:
    Impulse(NodeRef frequency = 1.0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef frequency;

private:
    std::vector<float> steps_remaining;
};

REGISTER(Impulse, "impulse")

}
