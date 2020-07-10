#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Line : public Node
{
public:
    Line(NodeRef from = 0.0, NodeRef to = 1.0, NodeRef time = 1.0);

    virtual void process(sample **out, int num_frames);

    NodeRef from;
    NodeRef to;
    NodeRef time;

private:
    float value;
    float value_change_per_step;
    int step;
    int duration_samples;
};

REGISTER(Line, "line")
}
