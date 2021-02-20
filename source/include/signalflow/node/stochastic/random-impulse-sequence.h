#include "signalflow/core/constants.h"
#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

class RandomImpulseSequence : public StochasticNode
{

public:
    RandomImpulseSequence(NodeRef probability = 0.5, NodeRef length = 8,
                          NodeRef clock = nullptr, NodeRef explore = nullptr, NodeRef reset = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    std::vector<int> sequence;
    std::vector<int> position;
    NodeRef probability;
    NodeRef length;
    NodeRef clock;
    NodeRef explore;
};

REGISTER(RandomImpulseSequence, "random-impulse-sequence")

}
