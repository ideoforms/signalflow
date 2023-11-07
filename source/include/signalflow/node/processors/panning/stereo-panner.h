#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Pans a mono input to a stereo output. Pans from -1 (hard left) to +1 (hard right),
 * with 0 = centre.
 *---------------------------------------------------------------------------------*/
class StereoPanner : public Node
{
public:
    StereoPanner(NodeRef input = 0, NodeRef pan = 0.0);

    virtual void process(Buffer &out, int num_frames) override;

    NodeRef input;
    NodeRef pan;
};

REGISTER(StereoPanner, "stereo-panner")
}
