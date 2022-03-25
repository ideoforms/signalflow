#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class BufferRecorder : public Node
{
public:
    BufferRecorder(BufferRef buffer = nullptr, NodeRef input = 0.0, NodeRef feedback = 0.0, bool loop = false);

    virtual void process(Buffer &out, int num_frames);
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0);

    BufferRef buffer;

    NodeRef input;
    NodeRef feedback;

private:
    float phase;
    bool loop;
};

REGISTER(BufferRecorder, "buffer-recorder")
}
