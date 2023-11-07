#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Each time a clock or trigger is received, outputs the next value in the sequence.
 * At all other times, outputs zero.
 *---------------------------------------------------------------------------------*/
class ImpulseSequence : public Node
{

public:
    ImpulseSequence(std::vector<int> sequence = std::vector<int>(), NodeRef clock = nullptr);
    ImpulseSequence(std::string sequence, NodeRef clock = nullptr);

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
