#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class FeedbackBufferWriter : public Node
{
public:
    FeedbackBufferWriter(BufferRef buffer = nullptr, NodeRef input = 0.0, NodeRef delay_time = 0.1);
    virtual void process(Buffer &out, int num_frames);

private:
    BufferRef buffer;
    NodeRef input;
    NodeRef delay_time;
    float phase;
};

REGISTER(FeedbackBufferWriter, "feedback-buffer-writer")
}
