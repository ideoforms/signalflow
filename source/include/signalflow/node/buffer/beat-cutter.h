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
               NodeRef duty_cycle = 1.0,
               NodeRef rate = 1.0,
               NodeRef segment_rate = 1.0);

    virtual void process(Buffer &out, int num_frames);
    virtual void trigger(std::string = SIGNALFLOW_DEFAULT_TRIGGER, float value = 0.0);
    virtual void set_buffer(std::string, BufferRef buffer);

protected:
    BufferRef buffer;
    int segment_count;
    NodeRef stutter_probability;
    NodeRef stutter_count;
    NodeRef jump_probability;
    NodeRef duty_cycle;
    NodeRef rate;
    NodeRef segment_rate;

    // List of offset positions for each segment's start point, in samples
    std::vector<int> segment_offsets;

    // Current phase, in samples, as an absolute position within the buffer
    float phase;
    // Index of the segment currently playing
    int segment_index;
    // Current phase of the segment currently playing, in samples
    float segment_phase;
    // Length of the segment currently playing, in samples
    int segment_length;
    // Duty cycle of the current segment, [0..1]
    float segment_duty;
    // Offset of the segment currently playing, in samples
    int current_segment_offset;
    // Offset of the next segment to play, in samples
    int next_segment_offset;
    // Length of current stutter segment, in samples
    int current_stutter_length;
    // Current segment rate, so that individual segments can be modulated discretely
    // This value is snapshot of the segment_rate input at the moment the segment playback begins
    float current_segment_rate;

    void set_segment(int segment_index, int frame = 0);
};

REGISTER(BeatCutter, "beat-cutter")
}
