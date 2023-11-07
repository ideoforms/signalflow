#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Counterpart to FeedbackBufferWriter.
 *---------------------------------------------------------------------------------*/
class FeedbackBufferReader : public Node
{
public:
    FeedbackBufferReader(BufferRef buffer = nullptr);

    virtual void set_buffer(std::string, BufferRef buffer);
    virtual void process(Buffer &out, int num_frames);

private:
    BufferRef buffer;
    double phase;
    double rate_scale_factor;
};

REGISTER(FeedbackBufferReader, "feedback-buffer-reader")
}
