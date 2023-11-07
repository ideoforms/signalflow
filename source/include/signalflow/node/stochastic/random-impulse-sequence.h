#include "signalflow/core/constants.h"
#include "signalflow/node/stochastic/stochastic-node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Generates a random sequence of 0/1 bits with the given length, and the given
 * probability each each bit = 1. The position of the sequence is incremented
 * on each clock signal. explore and generate are trigger inputs which cause
 * the sequence to mutate and re-generate respectively.
 *---------------------------------------------------------------------------------*/
class RandomImpulseSequence : public StochasticNode
{

public:
    RandomImpulseSequence(NodeRef probability = 0.5, NodeRef length = 8,
                          NodeRef clock = nullptr, NodeRef explore = nullptr, NodeRef generate = nullptr, NodeRef reset = nullptr);

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
    NodeRef generate;
};

REGISTER(RandomImpulseSequence, "random-impulse-sequence")

}
