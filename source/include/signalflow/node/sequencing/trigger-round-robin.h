#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Relay trigger() events to a single node from the list of connected outputs,
 * with `direction` determining the direction: 1 (or above) = move forwards by N,
 * -1 = move backwards by N, 0 = stationary.
 *---------------------------------------------------------------------------------*/
class TriggerRoundRobin : public Node
{

public:
    TriggerRoundRobin(NodeRef direction = 1);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;

private:
    unsigned int current_output_index = 0;
    NodeRef direction;
};

REGISTER(TriggerRoundRobin, "trigger-round-robin")

}
