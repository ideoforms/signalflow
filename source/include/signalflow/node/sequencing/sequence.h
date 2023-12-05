#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs the elements in `sequence`, incrementing position on each `clock`.
 *---------------------------------------------------------------------------------*/
class Sequence : public Node
{

public:
    Sequence(std::vector<float> sequence = std::vector<float>(), NodeRef clock = nullptr);

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    std::vector<float> sequence;
    std::vector<int> position;
    NodeRef clock;
};

REGISTER(Sequence, "sequence")

}
