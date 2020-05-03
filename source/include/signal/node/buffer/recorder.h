#include "signal/buffer/buffer.h"
#include "signal/core/constants.h"
#include "signal/node/node.h"

namespace libsignal
{
class Recorder : public Node
{
public:
    Recorder(BufferRef buffer = nullptr, NodeRef input = 0.0, bool loop = false);

    BufferRef buffer;

    NodeRef input;

    float phase;
    bool loop;

    virtual void process(sample **out, int num_frames);
};

REGISTER(Recorder, "recorder")
}
