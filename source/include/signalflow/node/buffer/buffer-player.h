#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Plays the contents of the given buffer. start_time/end_time are in seconds.
 * When a clock signal is received, rewinds to the start_time.
 *---------------------------------------------------------------------------------*/
class BufferPlayer : public Node
{
public:
    BufferPlayer(BufferRef buffer = nullptr, NodeRef rate = 1.0, NodeRef loop = 0,
                 NodeRef start_time = nullptr, NodeRef end_time = nullptr, NodeRef clock = nullptr);

    virtual void set_buffer(std::string, BufferRef buffer) override;
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0) override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual PropertyRef get_property(std::string name) override;

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
