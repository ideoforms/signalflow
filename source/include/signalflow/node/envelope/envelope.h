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
             std::vector<NodeRef> curves = std::vector<NodeRef>(),
             NodeRef clock = nullptr,
             bool loop = false);

    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    float level;
    unsigned int node_index;
    float node_phase;
    std::vector<NodeRef> levels;
    std::vector<NodeRef> times;
    std::vector<NodeRef> curves;
    NodeRef clock;
    bool loop;
};

REGISTER(Envelope, "envelope")

}
