#pragma once

#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/node/node.h"

#include <vector>

namespace signalflow
{
/**--------------------------------------------------------------------------------*
 * Feedback Delay Network reverb.
 * Uses multiple delay lines with an orthogonal feedback matrix for dense,
 * natural-sounding reverberation.
 *---------------------------------------------------------------------------------*/
class FeedbackDelayNetwork : public UnaryOpNode
{
public:
    FeedbackDelayNetwork(NodeRef input = 0.0,
                         int num_delays = 8,
                         NodeRef feedback = 0.85,
                         NodeRef damping = 0.5,
                         NodeRef wet = 0.5,
                         NodeRef dry = 0.5,
                         float max_delay_time = 0.2);
    ~FeedbackDelayNetwork() override;

    virtual void alloc() override;
    virtual void process(Buffer &out, int num_frames) override;

private:
    // Delay line configuration
    int num_delays;

    // Input parameters
    NodeRef feedback;
    NodeRef damping;
    NodeRef wet;
    NodeRef dry;

    float max_delay_time;
    std::vector<float> delay_times_array;

    // Delay line buffers and state (per-channel)
    std::vector<std::vector<SampleRingBuffer *>> delay_buffers;
    std::vector<std::vector<sample>> delay_outputs;
    std::vector<std::vector<sample>> lowpass_state;

    // Feedback matrix (Hadamard)
    std::vector<std::vector<sample>> feedback_matrix;

    // Helper methods
    void initialize_delay_times();
    void initialize_feedback_matrix();
    sample apply_damping(sample input, sample &state, sample damping_coeff);
};

REGISTER(FeedbackDelayNetwork, "feedback-delay-network")
}
