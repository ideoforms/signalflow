#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Produces a constant value.
 *---------------------------------------------------------------------------------*/
class Constant : public Node
{
public:
    Constant(sample value = 0);

    float value;
    virtual float get_value() override;
    virtual void set_value(float value) override;

    virtual void process(Buffer &out, int num_frames) override;
};

REGISTER(Constant, "constant")
}
