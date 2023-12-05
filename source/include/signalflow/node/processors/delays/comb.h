#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Comb delay, with `feedback` between 0 and 1.
 * `delay_time` must be less than or equal to `max_delay_time`.
 *---------------------------------------------------------------------------------*/
class CombDelay : public UnaryOpNode
{
public:
    CombDelay(NodeRef input = 0.0, NodeRef delay_time = 0.1, NodeRef feedback = 0.5, float max_delay_time = 0.5);
    ~CombDelay() override;

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef delay_time;
    NodeRef feedback;
    float max_delay_time;

    std::vector<SampleRingBuffer *> buffers;
};

REGISTER(CombDelay, "comb-delay")
}
