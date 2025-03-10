#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Writes a rolling history buffer of a given duration. At a given moment in time,
 * the contents of the buffer will be equal to the past N seconds of the audio
 * generated by `input`.
 *
 * This is useful for (e.g.) a visual display of a rolling waveform or LFO window.
 * `downsample` can be used to downsample the input; for example, with `downsample`
 * of 10, a 1-second buffer can be used to display 10 seconds of historical audio.
 *---------------------------------------------------------------------------------*/
class HistoryBufferWriter : public UnaryOpNode
{
public:
    HistoryBufferWriter(BufferRef buffer = nullptr, NodeRef input = 0.0, int downsample = 1);
    virtual void process(Buffer &out, int num_frames);

private:
    BufferRef buffer;
    int downsample;
};

REGISTER(HistoryBufferWriter, "feedback-buffer-writer")
}
