#pragma once

#include "signal/buffer/ringbuffer.h"
#include "signal/core/constants.h"
#include "signal/node/node.h"

#include <vector>

namespace libsignal
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
