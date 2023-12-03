#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Stutters the input whenever a trigger is received on `clock`.
 * Generates `stutter_count` repeats, with duration of `stutter_time`.
 *---------------------------------------------------------------------------------*/
class Stutter : public UnaryOpNode
{
public:
    Stutter(NodeRef input = 0.0,
            NodeRef stutter_time = 0.1,
            NodeRef stutter_count = 1,
            NodeRef clock = nullptr,
            float max_stutter_time = 1.0);
    ~Stutter() override;

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER, float value = 1.0) override;

private:
    NodeRef stutter_time;
    NodeRef stutter_count;
    NodeRef clock;
    float max_stutter_time;

    std::vector<SampleRingBuffer *> buffers;
    std::vector<int> stutter_index;
    std::vector<int> stutters_to_do;
    std::vector<int> stutter_sample_buffer_offset;
    std::vector<int> stutter_samples_remaining;
};

REGISTER(Stutter, "stutter")
}
