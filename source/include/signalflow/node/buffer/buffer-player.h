#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class BufferPlayer : public Node
{
public:
    BufferPlayer(BufferRef buffer = nullptr, NodeRef rate = 1.0, NodeRef loop = 0,
                 NodeRef start_time = nullptr, NodeRef end_time = nullptr, NodeRef clock = nullptr);

    virtual void set_buffer(std::string, BufferRef buffer);
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0);
    virtual void process(Buffer &out, int num_frames);

    BufferRef buffer;

    NodeRef rate = nullptr;
    NodeRef loop = nullptr;
    NodeRef start_time = nullptr;
    NodeRef end_time = nullptr;
    NodeRef clock = nullptr;

private:
    double phase;
    double rate_scale_factor;
};

REGISTER(BufferPlayer, "buffer-player")
}
