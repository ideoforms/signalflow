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
    OneTapDelay(NodeRef input = 0.0, NodeRef delaytime = 0.1, float maxdelaytime = 0.5);
    ~OneTapDelay();

    virtual void process(Buffer &out, int num_frames) override;

private:
    NodeRef delaytime;

    std::vector<SampleRingBuffer *> buffers;
};

REGISTER(OneTapDelay, "one-tap-delay")
}
