#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
class AllpassDelay : public UnaryOpNode
{
public:
    AllpassDelay(NodeRef input = 0.0, NodeRef delay_time = 0.1, NodeRef feedback = 0.5, float max_delay_time = 0.5);
    ~AllpassDelay() override;

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef delay_time;
    NodeRef feedback;

    std::vector<SampleRingBuffer *> buffers;
};

REGISTER(AllpassDelay, "allpass-delay")
}
