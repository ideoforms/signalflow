
#include "signalflow/core/graph.h"
#include "signalflow/node/analysis/yin.h"

#include <algorithm>
#include <cmath>
#include <stdlib.h>

namespace signalflow
{

/*--------------------------------------------------------------------------------
 * YIN Pitch Tracking Algorithm
 * 
 * Based on: "YIN, a fundamental frequency estimator for speech and music"
 * by Alain de Cheveigné and Hideki Kawahara (2002)
 * 
 * Steps:
 *  - Compute difference function d(tau) = sum((x[j] - x[j+tau])^2)
 *  - Compute cumulative mean normalized difference function (CMNDF)
 *  - Find first tau below threshold (with absolute minimum search)
 *  - Use parabolic interpolation for sub-sample accuracy
 *  - Convert tau to frequency: f0 = sample_rate / tau
 *--------------------------------------------------------------------------------*/

YinPitchTracker::YinPitchTracker(NodeRef input,
                                 NodeRef threshold,
                                 NodeRef f0_min,
                                 NodeRef f0_max,
                                 unsigned int window_size)
    : UnaryOpNode(input), threshold(threshold), f0_min(f0_min), f0_max(f0_max), window_size(window_size), current_f0(0.0f)
{
    SIGNALFLOW_CHECK_GRAPH();

    this->name = "yin-pitch-tracker";

    this->create_input("threshold", this->threshold);
    this->create_input("f0_min", this->f0_min);
    this->create_input("f0_max", this->f0_max);

    // Ring buffer for input history
    input_buffer = new SampleRingBuffer(window_size);

    // Allocate working arrays
    difference_function.resize(window_size / 2);
    cmndf.resize(window_size / 2);
}

YinPitchTracker::~YinPitchTracker()
{
    delete input_buffer;
}

void YinPitchTracker::compute_difference_function(int tau_min, int tau_max)
{
    // Step 1: Compute difference function
    // d(tau) = sum_{j=1}^{W} (x[j] - x[j+tau])^2

    for (int tau = tau_min; tau < tau_max; tau++)
    {
        float sum = 0.0f;
        for (int j = 0; j < (int) window_size / 2; j++)
        {
            // This line is currently the CPU bottleneck
            float delta = input_buffer->get(-j) - input_buffer->get(-j - tau);
            sum += delta * delta;
        }
        difference_function[tau] = sum;
    }
}

void YinPitchTracker::compute_cmndf(int tau_min, int tau_max)
{
    // Step 2: Compute Cumulative Mean Normalized Difference Function
    // cmndf(tau) = d(tau) / [(1/tau) * sum_{j=1}^{tau} d(j)]

    cmndf[0] = 1.0f;

    float running_sum = 0.0f;
    for (int tau = 1; tau < tau_max; tau++)
    {
        running_sum += difference_function[tau];
        if (running_sum > 0.0f)
        {
            cmndf[tau] = difference_function[tau] / (running_sum / tau);
        }
        else
        {
            // Avoid division by zero
            cmndf[tau] = 1.0f;
        }
    }
}

int YinPitchTracker::find_absolute_minimum(int tau_min, int tau_max, float threshold_val)
{
    // Step 3: Absolute threshold and minimum search
    // Find the first tau where CMNDF drops below threshold
    // If no such tau exists, return the tau with the minimum CMNDF value

    int tau = tau_min;

    // Search for first value below threshold
    while (tau < tau_max)
    {
        if (cmndf[tau] < threshold_val)
        {
            // Found a dip below threshold
            // Now search for local minimum
            while (tau + 1 < tau_max && cmndf[tau + 1] < cmndf[tau])
            {
                tau++;
            }
            return tau;
        }
        tau++;
    }

    // No value below threshold found, return absolute minimum
    int min_tau = tau_min;
    float min_val = cmndf[min_tau];
    for (tau = tau_min + 1; tau < tau_max; tau++)
    {
        if (cmndf[tau] < min_val)
        {
            min_val = cmndf[tau];
            min_tau = tau;
        }
    }

    return min_tau;
}

float YinPitchTracker::parabolic_interpolation(int tau)
{
    // Step 4: Parabolic interpolation for sub-sample accuracy
    // Fits a parabola through the minimum and its neighbors
    // to get a more accurate estimate of the true minimum position

    if (tau < 1 || tau >= (int) difference_function.size() - 1)
    {
        return (float) tau; // Can't interpolate at boundaries
    }

    float s0 = cmndf[tau - 1];
    float s1 = cmndf[tau];
    float s2 = cmndf[tau + 1];

    // Parabolic interpolation formula
    float adjustment = 0.5f * (s0 - s2) / (s0 - 2.0f * s1 + s2);

    // Check for valid adjustment
    if (std::isfinite(adjustment) && std::abs(adjustment) < 1.0f)
    {
        return tau + adjustment;
    }

    return (float) tau;
}

void YinPitchTracker::process(Buffer &out, int num_frames)
{
    SIGNALFLOW_CHECK_GRAPH();

    for (int frame = 0; frame < num_frames; frame++)
    {
        // Get input from channel 0 only (monophonic)
        float input_sample = this->input->out[0][frame];

        // Add to ring buffer
        input_buffer->append(input_sample);
    }

    // Get current parameter values (from channel 0)
    float threshold_val = this->threshold->out[0][0];
    float f0_min_val = this->f0_min->out[0][0];
    float f0_max_val = this->f0_max->out[0][0];

    // Convert frequency bounds to tau (lag) bounds
    float sample_rate = this->graph->get_sample_rate();
    int tau_max = (int) (sample_rate / f0_min_val);
    int tau_min = (int) (sample_rate / f0_max_val);

    // Clamp to valid range
    tau_min = std::max(1, tau_min);
    tau_max = std::min((int) (window_size / 2) - 1, tau_max);

    // TODO: check if tau_max <= tau_min

    // Step 1: Compute difference function (slow!)
    compute_difference_function(tau_min, tau_max);

    // Step 2: Compute CMNDF
    compute_cmndf(tau_min, tau_max);

    // Step 3: Find minimum
    int tau = find_absolute_minimum(tau_min, tau_max, threshold_val);

    // Step 4: Parabolic interpolation
    float tau_interpolated = parabolic_interpolation(tau);

    // Step 5: Convert to frequency
    if (tau_interpolated > 0.0f && cmndf[tau] < 1.0f)
    {
        current_f0 = sample_rate / tau_interpolated;

        // Sanity check - clamp to valid range
        current_f0 = std::max(f0_min_val, std::min(f0_max_val, current_f0));
    }
    // else: keep previous f0 (no confident pitch detected)

    // Output estimated f0 to all channels
    for (int frame = 0; frame < num_frames; frame++)
    {
        for (int channel = 0; channel < this->num_output_channels; channel++)
        {
            out[channel][frame] = current_f0;
        }
    }
}

}
