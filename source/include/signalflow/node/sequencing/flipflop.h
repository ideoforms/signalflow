#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Flips from 0/1 on each clock.
 *---------------------------------------------------------------------------------*/
class FlipFlop : public Node
{

public:
    FlipFlop(NodeRef clock = 0);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

    NodeRef clock;

private:
    std::vector<bool> value;
};

REGISTER(FlipFlop, "flipflop")

}
