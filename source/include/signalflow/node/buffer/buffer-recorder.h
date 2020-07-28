#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class BufferRecorder : public Node
{
public:
    BufferRecorder(BufferRef buffer = nullptr, NodeRef input = 0.0, NodeRef feedback = 0.0, bool loop = false);

    BufferRef buffer;

    NodeRef input;
    NodeRef feedback;

    float phase;
    bool loop;

    virtual void process(sample **out, int num_frames);
};

REGISTER(BufferRecorder, "buffer-recorder")
}
