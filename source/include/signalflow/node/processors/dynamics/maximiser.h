#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Gain maximiser.
 *---------------------------------------------------------------------------------*/
class Maximiser : public UnaryOpNode
{
public:
    Maximiser(NodeRef input = 0.0,
              NodeRef ceiling = 0.5,
              NodeRef attack_time = 1.0,
              NodeRef release_time = 1.0);

    virtual void process(Buffer &out, int num_frames);

    NodeRef ceiling;
    NodeRef attack_time;
    NodeRef release_time;

private:
    float gain;
};

REGISTER(Maximiser, "maximiser")
}
