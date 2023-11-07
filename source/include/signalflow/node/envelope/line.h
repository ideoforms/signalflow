#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Line segment with the given start/end values and duration.
 * If loop is true, repeats indefinitely.
 * Retriggers on a clock signal.
 *---------------------------------------------------------------------------------*/
class Line : public Node
{
public:
    Line(NodeRef from = 0.0, NodeRef to = 1.0, NodeRef time = 1.0, NodeRef loop = 0, NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;
    virtual void process(Buffer &out, int num_frames) override;

    NodeRef from;
    NodeRef to;
    NodeRef time;
    NodeRef loop;
    NodeRef clock;

private:
    std::vector<float> value;
    std::vector<float> value_change_per_step;
    std::vector<int> step;
    std::vector<int> duration_samples;
};

REGISTER(Line, "line")
}
