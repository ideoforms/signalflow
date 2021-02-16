#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

class ImpulseSequence : public Node
{

public:
    ImpulseSequence(std::vector<int> sequence = std::vector<int>(), NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    std::vector<int> sequence;
    std::vector<int> position;
    NodeRef clock;
};

REGISTER(ImpulseSequence, "impulse-sequence")

}
