#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Implementation of Dan Stowell's Squiz algorithm, a kind of downsampler.
 *---------------------------------------------------------------------------------*/
class Squiz : public UnaryOpNode
{
public:
    Squiz(NodeRef input = 0.0, NodeRef rate = 2.0, NodeRef chunk_size = 1);

    virtual void process(Buffer &out, int num_frames);

    NodeRef rate;
    NodeRef chunk_size;

    int buffer_length;
    std::vector<std::vector<sample>> buffers;
    std::vector<int> write_pos;
    std::vector<float> read_pos;
    std::vector<sample> last_input;
    std::vector<int> zero_crossings;
};

REGISTER(Squiz, "squiz")
}
