#pragma once

#include "signalflow/node/node.h"

namespace signalflow
{
class RandomBrownian : public Node
{
public:
    RandomBrownian(NodeRef min = -1.0, NodeRef max = 1.0, NodeRef delta = 0.01, NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;

private:
    NodeRef min;
    NodeRef max;
    NodeRef delta;
    NodeRef clock;

    std::vector<sample> value;
};

REGISTER(RandomBrownian, "random-brownian")
}
