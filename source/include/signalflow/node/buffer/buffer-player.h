#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#define SIGNALFLOW_SAMPLER_TRIGGER_SET_POSITION "position"

#define SIGNALFLOW_SAMPLER_TRIGGER_SET_LOOP_START "loop_start"
#define SIGNALFLOW_SAMPLER_TRIGGER_SET_LOOP_END "loop_end"

namespace signalflow
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

    virtual void set_buffer(std::string, BufferRef buffer);
    virtual void trigger(std::string = SIGNALFLOW_SAMPLER_TRIGGER_SET_POSITION, float value = 0.0);
    virtual void process(Buffer &out, int num_frames);

private:
    float phase;
};

REGISTER(BufferPlayer, "buffer-player")
}
