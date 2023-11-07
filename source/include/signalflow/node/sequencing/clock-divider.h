#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * When given a clock input (e.g., an Impulse), divides the clock by the given
 * factor. factor must be an integer greater than or equal to 1.
 *---------------------------------------------------------------------------------*/
class ClockDivider : public Node
{

public:
    ClockDivider(NodeRef clock = 0, NodeRef factor = 1);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

    NodeRef clock;
    NodeRef factor;

private:
    std::vector<int> counter;
};

REGISTER(ClockDivider, "clock-divider")

}
