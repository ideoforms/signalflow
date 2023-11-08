#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Outputs the cross-correlation of the input signal with the given buffer.
 * If hop_size is zero, calculates the cross-correlation every sample.
 *---------------------------------------------------------------------------------*/
class CrossCorrelate : public UnaryOpNode
{
public:
    CrossCorrelate(NodeRef input = nullptr, BufferRef buffer = nullptr, int hop_size = 0);

    BufferRef buffer;
    int hop_size = 0;

    virtual void process(Buffer &out, int num_frames);

private:
    SampleRingBuffer *ring_buffer;
};

REGISTER(CrossCorrelate, "cross-correlate")
}
