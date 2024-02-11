#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * TimeShift
 *---------------------------------------------------------------------------------*/
class TimeShift : public UnaryOpNode
{

public:
    TimeShift(NodeRef a = 0);

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

protected:
    int num_shifts_queued = 0;
};

REGISTER(TimeShift, "time-shift")

}
