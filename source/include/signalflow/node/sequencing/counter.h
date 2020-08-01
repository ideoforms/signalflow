#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Counter : public Node
{

public:
    Counter(NodeRef clock = 0, NodeRef min = 0, NodeRef max = 0);

    virtual void process(sample **out, int num_frames);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0);

    NodeRef clock;
    NodeRef min;
    NodeRef max;

    int counter[SIGNALFLOW_MAX_CHANNELS];
};

REGISTER(Counter, "counter")

}
