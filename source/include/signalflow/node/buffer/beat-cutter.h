#include "signalflow/buffer/buffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

namespace signalflow
{
class BeatCutter : public Node
{
public:
    BeatCutter(BufferRef buffer = nullptr,
               int segment_count = 8,
               NodeRef stutter_probability = 0.0,
               NodeRef stutter_count = 1,
               NodeRef jump_probability = 0.0,
               NodeRef duty_cycle = 1.0);

    virtual void process(Buffer &out, int num_frames);

protected:
    BufferRef buffer;
    int segment_count;
    NodeRef stutter_probability;
    NodeRef stutter_count;
    NodeRef jump_probability;
    NodeRef duty_cycle;

    std::vector<int> segment_offsets;
    int phase;
    int segment_index;
    int segment_phase;
    int segment_length;
    float segment_duty;
    int current_segment_offset;
    int next_segment_offset;

    int current_stutter_length;
};

REGISTER(BeatCutter, "beat-cutter")
}
