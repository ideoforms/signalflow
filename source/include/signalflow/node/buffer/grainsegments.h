#pragma once

#include "granulator.h"
#include "signalflow/buffer/buffer.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
class GrainSegments : public Node
{
public:
    GrainSegments(BufferRef buffer = nullptr,
                  NodeRef clock = 0,
                  NodeRef target = 0,
                  PropertyRef offsets = {},
                  PropertyRef values = {},
                  PropertyRef durations = {});

    BufferRef buffer;
    BufferRef envelope;

    NodeRef clock;
    NodeRef target;
    PropertyRef offsets;
    PropertyRef values;
    PropertyRef durations;

    virtual void process(Buffer &out, int num_frames);

private:
    sample clock_last;

    std::vector<Grain *> grains;
};

REGISTER(GrainSegments, "grain-segments")
}
