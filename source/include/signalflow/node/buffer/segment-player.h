#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Trigger segments of a buffer at the given onset positions.
 *---------------------------------------------------------------------------------*/
class SegmentPlayer : public Node
{
public:
    SegmentPlayer(BufferRef buffer = nullptr, std::vector<float> onsets = {});

    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

    BufferRef buffer;
    PropertyRef onsets;
    float phase;
};

REGISTER(SegmentPlayer, "segment-player")
}
