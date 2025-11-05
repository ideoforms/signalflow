#include "signalflow/core/graph.h"
#include "signalflow/node/oscillators/constant.h"
#include "signalflow/node/processors/reverb/fdn.h"
#include <algorithm>
#include <cmath>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 * Available input parameters:
 * - input: Input signal to be reverberated
 * - num_delays: Number of delay lines in the network (typically 4, 8, or 16)
 * - delay_times: Array of delay times in seconds for each delay line
 * - feedback: Overall feedback amount (0.0 to 1.0, typically 0.85-0.99 for reverb)
 * - damping: High-frequency damping factor (0.0 to 1.0)
 * - wet: Wet (reverb) level (0.0 to 1.0)
 * - dry: Dry (input) level (0.0 to 1.0)
 * 
 * Class properties needed:
 * 
 * Main state:
 * - std::vector<SampleRingBuffer *> delay_buffers: Delay line buffers
 * - std::vector<sample> delay_outputs: Current outputs from each delay line
 * - std::vector<sample> lowpass_state: State for one-pole lowpass filters (damping)
 * 
 * Matrices for the reverb:
 * - std::vector<std::vector<sample>> feedback_matrix: Orthogonal feedback matrix (Hadamard or Householder)
 * - int num_delays: Number of delay lines
 * - std::vector<float> delay_times_array: Delay times for each line
 * 
 * Filter parameters:
 * - NodeRef feedback: Feedback coefficient
 * - NodeRef damping: Damping coefficient for high-frequency absorption
 * - NodeRef wet: Wet mix level
 * - NodeRef dry: Dry mix level
 * - float max_delay_time: Maximum delay time for buffer allocation
 *--------------------------------------------------------------------------------*/

FeedbackDelayNetwork::FeedbackDelayNetwork(NodeRef input,
                                           int num_delays,
                                           NodeRef feedback,
                                           NodeRef damping,
                                           NodeRef wet,
                                           NodeRef dry,
                                           float max_delay_time)
    : UnaryOpNode(input), num_delays(num_delays), feedback(feedback), damping(damping), wet(wet), dry(dry), max_delay_time(max_delay_time)
{
    this->name = "feedback-delay-network";

    this->create_input("feedback", this->feedback);
    this->create_input("damping", this->damping);
    this->create_input("wet", this->wet);
    this->create_input("dry", this->dry);

    SIGNALFLOW_CHECK_GRAPH();

    // Initialize delay times with prime-related values for decorrelation
    initialize_delay_times();

    // Create delay buffers for each channel and delay line
    delay_buffers.resize(SIGNALFLOW_MAX_CHANNELS);
    delay_outputs.resize(SIGNALFLOW_MAX_CHANNELS);
    lowpass_state.resize(SIGNALFLOW_MAX_CHANNELS);

    for (int channel = 0; channel < SIGNALFLOW_MAX_CHANNELS; channel++)
    {
        delay_buffers[channel].resize(num_delays);
        delay_outputs[channel].resize(num_delays, 0.0);
        lowpass_state[channel].resize(num_delays, 0.0);

        for (int i = 0; i < num_delays; i++)
        {
            delay_buffers[channel][i] = new SampleRingBuffer(max_delay_time * this->graph->get_sample_rate());
        }
    }

    // Initialize feedback matrix
    initialize_feedback_matrix();
}

FeedbackDelayNetwork::~FeedbackDelayNetwork()
{
    for (int channel = 0; channel < SIGNALFLOW_MAX_CHANNELS; channel++)
    {
        for (auto buffer : delay_buffers[channel])
        {
            delete buffer;
        }
    }
}

void FeedbackDelayNetwork::initialize_delay_times()
{
    // Use prime-related delays for good decorrelation
    // These are scaled to fit within max_delay_time
    const float prime_ratios[] = { 1.0f, 1.19f, 1.41f, 1.67f, 1.97f, 2.27f, 2.59f, 2.97f,
                                   3.37f, 3.79f, 4.27f, 4.73f, 5.23f, 5.79f, 6.31f, 6.89f };

    delay_times_array.clear();
    for (int i = 0; i < num_delays; i++)
    {
        // Scale delays to use about 50-100% of max_delay_time
        float ratio = prime_ratios[i % 16];
        float normalized = ratio / prime_ratios[std::min(num_delays - 1, 15)];
        delay_times_array.push_back(0.5f * max_delay_time + 0.5f * max_delay_time * normalized);
    }
}

void FeedbackDelayNetwork::initialize_feedback_matrix()
{
    // Create a Hadamard matrix for orthogonal mixing
    // This ensures energy is conserved and decorrelates the signals

    feedback_matrix.resize(num_delays);
    for (int i = 0; i < num_delays; i++)
    {
        feedback_matrix[i].resize(num_delays);
    }

    if (num_delays == 4)
    {
        // 4x4 Hadamard matrix
        sample h[4][4] = {
            { 1, 1, 1, 1 },
            { 1, -1, 1, -1 },
            { 1, 1, -1, -1 },
            { 1, -1, -1, 1 }
        };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Normalize by 1/2
                feedback_matrix[i][j] = h[i][j] * 0.5;
            }
        }
    }
    else if (num_delays == 8)
    {
        // 8x8 Hadamard matrix
        sample h[8][8] = {
            { 1, 1, 1, 1, 1, 1, 1, 1 },
            { 1, -1, 1, -1, 1, -1, 1, -1 },
            { 1, 1, -1, -1, 1, 1, -1, -1 },
            { 1, -1, -1, 1, 1, -1, -1, 1 },
            { 1, 1, 1, 1, -1, -1, -1, -1 },
            { 1, -1, 1, -1, -1, 1, -1, 1 },
            { 1, 1, -1, -1, -1, -1, 1, 1 },
            { 1, -1, -1, 1, -1, 1, 1, -1 }
        };
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Normalize by 1/sqrt(8)
                feedback_matrix[i][j] = h[i][j] / std::sqrt(8.0);
            }
        }
    }
    else
    {
        // For other sizes, use a simple rotation matrix with sign flips
        for (int i = 0; i < num_delays; i++)
        {
            for (int j = 0; j < num_delays; j++)
            {
                if (i == j)
                    feedback_matrix[i][j] = 0.0;
                else if (j == (i + 1) % num_delays)
                    feedback_matrix[i][j] = 1.0 / std::sqrt(2.0);
                else if (j == (i + num_delays - 1) % num_delays)
                    feedback_matrix[i][j] = ((i % 2) == 0 ? 1.0 : -1.0) / std::sqrt(2.0);
                else
                    feedback_matrix[i][j] = 0.0;
            }
        }
    }
}

sample FeedbackDelayNetwork::apply_damping(sample input, sample &state, sample damping_coeff)
{
    // One-pole lowpass filter for high-frequency damping
    // y[n] = (1 - damping) * x[n] + damping * y[n-1]
    state = (1.0 - damping_coeff) * input + damping_coeff * state;
    return state;
}

void FeedbackDelayNetwork::alloc()
{
    // Reinitialize if needed
}

void FeedbackDelayNetwork::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int channel = 0; channel < this->num_input_channels; channel++)
    {
        for (int frame = 0; frame < num_frames; frame++)
        {
            // Get parameter values for this frame
            float feedback = this->feedback->out[channel][frame];
            float damping = this->damping->out[channel][frame];
            float wet = this->wet->out[channel][frame];
            float dry = this->dry->out[channel][frame];

            // Get input sample
            sample input_sample = this->input->out[channel][frame];

            // Read from all delay lines for this channel
            for (int i = 0; i < num_delays; i++)
            {
                float delay_samples = delay_times_array[i] * this->graph->get_sample_rate();
                delay_outputs[channel][i] = delay_buffers[channel][i]->get(-delay_samples);
            }

            // Apply feedback matrix to create new inputs for delay lines
            std::vector<sample> new_inputs(num_delays, 0.0);
            for (int i = 0; i < num_delays; i++)
            {
                sample sum = 0.0;
                for (int j = 0; j < num_delays; j++)
                {
                    sum += feedback_matrix[i][j] * delay_outputs[channel][j];
                }
                new_inputs[i] = sum;
            }

            // Write to delay lines with input injection, feedback, and damping
            for (int i = 0; i < num_delays; i++)
            {
                // Inject input into first few delay lines for spatial width
                float input_inject = (i < 4) ? input_sample * 0.25 : 0.0;

                // Apply feedback
                float delayed = new_inputs[i] * feedback + input_inject;

                // Apply damping (lowpass filter)
                delayed = apply_damping(delayed, lowpass_state[channel][i], damping);

                // Write to delay buffer
                delay_buffers[channel][i]->append(delayed);
            }

            // Sum delay outputs for reverb signal
            sample reverb_sum = 0.0;
            for (int i = 0; i < num_delays; i++)
            {
                reverb_sum += delay_outputs[channel][i];
            }

            // Normalize by number of delays to prevent level buildup
            reverb_sum /= std::sqrt((float) num_delays);

            // Mix wet and dry signals
            out[channel][frame] = dry * input_sample + wet * reverb_sum;
        }
    }
}

}
