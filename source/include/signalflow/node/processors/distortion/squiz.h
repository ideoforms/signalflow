#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Squiz : public UnaryOpNode
{
public:
    Squiz(NodeRef input = 0.0, NodeRef rate = 2.0, NodeRef chunk_size = 1);

    virtual void process(Buffer &out, int num_frames);

    NodeRef rate;
    NodeRef chunk_size;

    std::vector<BufferRef> buffers;
    std::vector<float> read_pos;
    std::vector<int> read_pos_chunk_counter;
    std::vector<float> repitched_read_pos;
    std::vector<int> repitched_read_pos_chunk_counter;
    std::vector<int> write_pos;
};

REGISTER(Squiz, "squiz")
}
