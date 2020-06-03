#pragma once

#include "signal/buffer/ringbuffer.h"
#include "signal/core/constants.h"
#include "signal/core/graph.h"
#include "signal/node/node.h"

#include <vector>

namespace libsignal
{
class CombDelay : public UnaryOpNode
{
public:
    CombDelay(NodeRef input = 0.0, NodeRef delaytime = 0.1, NodeRef feedback = 0.5, float maxdelaytime = 0.5);
    ~CombDelay();

    NodeRef delaytime;
    NodeRef feedback;
    float maxdelaytime;

    std::vector<SampleRingBuffer *> buffers;

    virtual void process(sample **out, int num_frames);
};

REGISTER(CombDelay, "comb-delay")
}
