#pragma once

#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Envelope : public Node
{
public:
    Envelope(std::vector<NodeRef> levels = std::vector<NodeRef>(),
             std::vector<NodeRef> times = std::vector<NodeRef>(),
             std::vector<NodeRef> curves = std::vector<NodeRef>());

    virtual void process(Buffer &out, int num_frames);

private:
    float level;
    int node_index;
    float node_phase;
    std::vector<NodeRef> levels;
    std::vector<NodeRef> times;
    std::vector<NodeRef> curves;
};

REGISTER(Envelope, "envelope")

}
