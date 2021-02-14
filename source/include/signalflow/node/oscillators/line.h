#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class Line : public Node
{
public:
    Line(NodeRef from = 0.0, NodeRef to = 1.0, NodeRef time = 1.0, NodeRef loop = 0);

    virtual void alloc() override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef from;
    NodeRef to;
    NodeRef time;
    NodeRef loop;

private:
    std::vector<float> value;
    std::vector<float> value_change_per_step;
    std::vector<int> step;
    std::vector<int> duration_samples;
};

REGISTER(Line, "line")
}
