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

    NodeRef delaytime;
    float maxdelaytime;

    std::vector<SampleRingBuffer *> buffers;

    virtual void process(sample **out, int num_frames);
};

REGISTER(OneTapDelay, "one-tap-delay")
}
