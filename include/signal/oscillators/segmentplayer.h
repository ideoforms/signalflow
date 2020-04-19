#include "signal/node.h"
#include "signal/constants.h"
#include "signal/buffer.h"

namespace libsignal
{
    class SegmentPlayer : public Node
    {
        public:
            SegmentPlayer(BufferRef buffer = nullptr, PropertyRef onsets = {});

            BufferRef buffer;

            float phase;

            virtual void process(sample **out, int num_frames);
            virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1.0);
    };

    REGISTER(SegmentPlayer, "segment_player");
}
