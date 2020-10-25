#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Counter : public Node
{

public:
    Counter(NodeRef clock = 0, NodeRef min = 0, NodeRef max = 0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

    NodeRef clock;
    NodeRef min;
    NodeRef max;

private:
    std::vector<int> counter;
};

REGISTER(Counter, "counter")

}
