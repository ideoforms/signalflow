#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
class OneTapDelay : public UnaryOpNode
{
public:
    OneTapDelay(NodeRef input = 0.0, NodeRef delay_time = 0.1, float max_delay_time = 0.5);
    ~OneTapDelay() override;

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef delay_time;

    std::vector<SampleRingBuffer *> buffers;
};

REGISTER(OneTapDelay, "one-tap-delay")
}
