#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class Latch : public Node
{

public:
    Latch(NodeRef set = 0, NodeRef reset = 0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    NodeRef set;
    NodeRef reset;
    std::vector<bool> value;
};

REGISTER(Latch, "latch")

}
