#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Accumulator with decay.
 *---------------------------------------------------------------------------------*/
class Accumulator : public Node
{
public:
    Accumulator(NodeRef strike_force = 0.5, NodeRef decay_coefficient = 0.9999, NodeRef trigger = nullptr);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = SIGNALFLOW_NULL_FLOAT) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef strike_force;
    NodeRef decay_coefficient;
    NodeRef _trigger;
    double current_value;
};

REGISTER(Accumulator, "accumulator")

}
