#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Distribute any triggers to all output nodes.
 *---------------------------------------------------------------------------------*/
class TriggerMult : public UnaryOpNode
{

public:
    TriggerMult(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT) override;
};

REGISTER(TriggerMult, "trigger-mult")

}
