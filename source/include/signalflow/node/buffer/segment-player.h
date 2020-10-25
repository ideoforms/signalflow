#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class SegmentPlayer : public Node
{
public:
    SegmentPlayer(BufferRef buffer = nullptr, PropertyRef onsets = {});

    BufferRef buffer;

    float phase;

    virtual void process(Buffer &out, int num_frames);
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0);
};

REGISTER(SegmentPlayer, "segment-player")
}
