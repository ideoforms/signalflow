#pragma once

#include "signal/buffer/ringbuffer.h"
#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{

class Squiz : public UnaryOpNode
{
public:
    Squiz(NodeRef input = 0.0, NodeRef rate = 2.0);

    virtual void process(sample **out, int num_frames);

    NodeRef rate;
    std::vector<BufferRef> buffers;
    std::vector<float> read_pos;
    std::vector<float> read_pos_accelerated;
    std::vector<int> write_pos;
    std::vector<bool> finished_chunk;
};

REGISTER(Squiz, "squiz")
}
