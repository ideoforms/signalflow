#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
class CombDelay : public UnaryOpNode
{
public:
    CombDelay(NodeRef input = 0.0, NodeRef delaytime = 0.1, NodeRef feedback = 0.5, float maxdelaytime = 0.5);
    ~CombDelay();

    virtual void process(sample **out, int num_frames) override;

private:
    NodeRef delaytime;
    NodeRef feedback;

    std::vector<SampleRingBuffer *> buffers;
};

REGISTER(CombDelay, "comb-delay")
}
