#include "signal/buffer/buffer.h"
#include "signal/core/constants.h"
#include "signal/node/node.h"

#define SIGNAL_SAMPLER_TRIGGER_SET_POSITION "position"

#define SIGNAL_SAMPLER_TRIGGER_SET_LOOP_START "loop_start"
#define SIGNAL_SAMPLER_TRIGGER_SET_LOOP_END "loop_end"

namespace libsignal
{
class BufferPlayer : public Node
{
public:
    BufferPlayer(BufferRef buffer = nullptr, NodeRef rate = 1.0, NodeRef loop = 0);

    BufferRef buffer;

    NodeRef input = nullptr;
    NodeRef rate = nullptr;

    NodeRef loop = nullptr;
    NodeRef loop_start = nullptr;
    NodeRef loop_end = nullptr;

    virtual void trigger(std::string = SIGNAL_SAMPLER_TRIGGER_SET_POSITION, float value = 0.0);
    virtual void process(sample **out, int num_frames);

private:
    float phase;
};

REGISTER(BufferPlayer, "buffer-player")
}
