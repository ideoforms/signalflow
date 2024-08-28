#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{

/**--------------------------------------------------------------------------------*
 * Read and write from a buffer concurrently, with controllable overdub.
 *---------------------------------------------------------------------------------*/
class BufferLooper : public Node
{
public:
    BufferLooper(BufferRef buffer = nullptr,
                 NodeRef input = 0.0,
                 NodeRef feedback = 0.0,
                 NodeRef loop_playback = 0,
                 NodeRef loop_record = 0,
                 NodeRef start_time = nullptr,
                 NodeRef end_time = nullptr,
                 NodeRef looper_level = 1.0,
                 NodeRef playthrough_level = 0.0);

    virtual void process(Buffer &out, int num_frames);
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0);

    BufferRef buffer;

private:
    float playback_phase;
    float record_phase;

    NodeRef input;
    NodeRef feedback;
    NodeRef loop_playback;
    NodeRef loop_record;
    NodeRef start_time;
    NodeRef end_time;
    NodeRef looper_level;
    NodeRef playthrough_level;

    bool is_recording;
    bool is_playing;
};

REGISTER(BufferLooper, "buffer-looper")
}
